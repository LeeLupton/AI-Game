#include "Blockforge/Core/Error.h"
#include "Blockforge/Core/Log.h"
#include <SDL.h>
#include <spdlog/spdlog.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string_view>
#include <chrono>
#include <cstdlib>
#include <memory>

namespace {

constexpr int kDefaultWindowWidth = 1280;
constexpr int kDefaultWindowHeight = 720;
constexpr std::chrono::milliseconds kBootstrapRunTime{2000};

struct WindowMetrics {
    int logicalWidth = 0;
    int logicalHeight = 0;
    int pixelWidth = 0;
    int pixelHeight = 0;
    float dpiScaleX = 1.0F;
    float dpiScaleY = 1.0F;
    float ddpi = 0.0F;
    float hdpi = 0.0F;
    float vdpi = 0.0F;
};

struct SdlQuitter {
    ~SdlQuitter()
    {
        SDL_Quit();
    }
};

using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

WindowPtr createWindow()
{
    constexpr auto flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* rawWindow = SDL_CreateWindow(
        "Blockforge Prototype",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kDefaultWindowWidth,
        kDefaultWindowHeight,
        flags);
    return WindowPtr(rawWindow, SDL_DestroyWindow);
}

WindowMetrics queryWindowMetrics(SDL_Window* window)
{
    WindowMetrics metrics;

    if (window == nullptr) {
        return metrics;
    }

    SDL_GetWindowSize(window, &metrics.logicalWidth, &metrics.logicalHeight);

#if SDL_VERSION_ATLEAST(2, 26, 0)
    SDL_GetWindowSizeInPixels(window, &metrics.pixelWidth, &metrics.pixelHeight);
#else
    SDL_GL_GetDrawableSize(window, &metrics.pixelWidth, &metrics.pixelHeight);
#endif

    const int displayIndex = SDL_GetWindowDisplayIndex(window);
    if (displayIndex >= 0) {
        float diagonalDpi = 0.0F;
        if (SDL_GetDisplayDPI(displayIndex, &diagonalDpi, &metrics.hdpi, &metrics.vdpi) == 0) {
            metrics.ddpi = diagonalDpi;
        } else {
            metrics.ddpi = 96.0F;
            metrics.hdpi = 96.0F;
            metrics.vdpi = 96.0F;
        }
    } else {
        metrics.ddpi = 96.0F;
        metrics.hdpi = 96.0F;
        metrics.vdpi = 96.0F;
    }

    metrics.pixelWidth = std::max(metrics.pixelWidth, metrics.logicalWidth);
    metrics.pixelHeight = std::max(metrics.pixelHeight, metrics.logicalHeight);

    if (metrics.logicalWidth > 0) {
        metrics.dpiScaleX = static_cast<float>(metrics.pixelWidth) / static_cast<float>(metrics.logicalWidth);
    }
    if (metrics.logicalHeight > 0) {
        metrics.dpiScaleY = static_cast<float>(metrics.pixelHeight) / static_cast<float>(metrics.logicalHeight);
    }

    return metrics;
}

void logWindowMetrics(const WindowMetrics& metrics, std::string_view reason)
{
    spdlog::info(
        "{} — logical {}x{}, pixel {}x{}, scale {:.2f}x{:.2f}, DPI diag {:.1f}, horiz {:.1f}, vert {:.1f}",
        reason,
        metrics.logicalWidth,
        metrics.logicalHeight,
        metrics.pixelWidth,
        metrics.pixelHeight,
        metrics.dpiScaleX,
        metrics.dpiScaleY,
        metrics.ddpi,
        metrics.hdpi,
        metrics.vdpi);
}

bool pumpEvents(SDL_Window* window)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            spdlog::info("Received SDL_QUIT event. Terminating bootstrap loop.");
            return false;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                const WindowMetrics metrics = queryWindowMetrics(window);
                logWindowMetrics(metrics, "Window size changed (pixels)");
                spdlog::info("→ Renderer should recreate swapchain-sized resources.");
            } else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                spdlog::info(
                    "Window logical size changed to {}x{}",
                    event.window.data1,
                    event.window.data2);
#ifdef SDL_WINDOWEVENT_DPICHANGED
            } else if (event.window.event == SDL_WINDOWEVENT_DPICHANGED) {
                const WindowMetrics metrics = queryWindowMetrics(window);
                logWindowMetrics(metrics, "DPI changed");
                spdlog::info("→ Renderer should recreate swapchain-sized resources.");
#endif
            }
            break;
        default:
            break;
        }
    }

    return true;
}

}  // namespace

int main(int /*argc*/, char** /*argv*/)
{
    bf::log::initialize(spdlog::level::info);
    spdlog::info("Blockforge prototype bootstrap running.");

bool pumpEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            spdlog::info("Received SDL_QUIT event. Terminating bootstrap loop.");
            return false;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                spdlog::info(
                    "Window resized to {}x{}",
                    event.window.data1,
                    event.window.data2);
            }
            break;
        default:
            break;
        }
    }

    return true;
}

}  // namespace

int main(int /*argc*/, char** /*argv*/)
{
    bf::log::initialize(spdlog::level::info);
    spdlog::info("Blockforge prototype bootstrap running.");

    if (SDL_getenv("SDL_VIDEODRIVER") == nullptr) {
        SDL_setenv("SDL_VIDEODRIVER", "dummy", 0);
    }

    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        const bf::Error error{
            .category = "SDL2",
            .code = "Init",
            .message = SDL_GetError(),
            .context = "SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)",
        };
        spdlog::critical("{}", bf::formatError(error));
        bf::log::shutdown();
        return EXIT_FAILURE;
    }

    const SdlQuitter sdlQuitter;

    spdlog::info("SDL initialized using '{}' video driver.", SDL_GetCurrentVideoDriver());

    auto window = createWindow();
    if (!window) {
        const bf::Error error{
            .category = "SDL2",
            .code = "CreateWindow",
            .message = SDL_GetError(),
            .context = "SDL_CreateWindow(Blockforge Prototype)",
        };
        spdlog::critical("{}", bf::formatError(error));
        bf::log::shutdown();
        return EXIT_FAILURE;
    }

    const WindowMetrics initialMetrics = queryWindowMetrics(window.get());
    logWindowMetrics(initialMetrics, "Window created");
=======
    spdlog::info("Window created at {}x{} (high DPI allowed).", kDefaultWindowWidth, kDefaultWindowHeight);

    const auto startTime = std::chrono::steady_clock::now();
    const auto endTime = startTime + kBootstrapRunTime;

    while (std::chrono::steady_clock::now() < endTime) {
        if (!pumpEvents(window.get())) {
        if (!pumpEvents()) {
            break;
        }
        SDL_Delay(16);
    }

    spdlog::info("Shutting down Blockforge bootstrap.");
    bf::log::shutdown();
    return EXIT_SUCCESS;
}
