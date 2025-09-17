#include "Blockforge/Core/Error.h"
#include "Blockforge/Core/Log.h"

#include <SDL.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <memory>

namespace {

constexpr int kDefaultWindowWidth = 1280;
constexpr int kDefaultWindowHeight = 720;
constexpr std::chrono::milliseconds kBootstrapRunTime{2000};

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

    spdlog::info("Window created at {}x{} (high DPI allowed).", kDefaultWindowWidth, kDefaultWindowHeight);

    const auto startTime = std::chrono::steady_clock::now();
    const auto endTime = startTime + kBootstrapRunTime;

    while (std::chrono::steady_clock::now() < endTime) {
        if (!pumpEvents()) {
            break;
        }
        SDL_Delay(16);
    }

    spdlog::info("Shutting down Blockforge bootstrap.");
    bf::log::shutdown();
    return EXIT_SUCCESS;
}
