#include "Blockforge/Core/Log.h"

#include <mutex>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace bf::log {
namespace {
std::shared_ptr<spdlog::logger> gLogger;
std::mutex gLoggerMutex;
}  // namespace

void initialize(spdlog::level::level_enum level)
{
    std::lock_guard<std::mutex> guard(gLoggerMutex);
    if (gLogger) {
        gLogger->set_level(level);
        return;
    }

    auto logger = spdlog::stdout_color_mt("Blockforge");
    logger->set_level(level);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%t][%l][%n] %v");

    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::warn);
    gLogger = std::move(logger);
}

std::shared_ptr<spdlog::logger> get()
{
    std::lock_guard<std::mutex> guard(gLoggerMutex);
    return gLogger;
}

void shutdown()
{
    std::lock_guard<std::mutex> guard(gLoggerMutex);
    if (!gLogger) {
        return;
    }

    spdlog::drop(gLogger->name());
    gLogger.reset();
    spdlog::shutdown();
}

}  // namespace bf::log
