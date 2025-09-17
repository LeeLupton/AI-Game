#pragma once

#include <memory>

#include <spdlog/logger.h>

namespace bf::log {

//! Initializes the global logger used across the engine.
void initialize(spdlog::level::level_enum level = spdlog::level::info);

//! Returns the shared logger instance. initialize() must be called first.
std::shared_ptr<spdlog::logger> get();

//! Tears down logging resources to flush logs during shutdown.
void shutdown();

}  // namespace bf::log
