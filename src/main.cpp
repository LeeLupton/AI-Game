#include "Blockforge/Core/Error.h"
#include "Blockforge/Core/Log.h"

#include <spdlog/spdlog.h>

int main()
{
    bf::log::initialize(spdlog::level::debug);

    spdlog::info("Blockforge prototype bootstrap running.");
    spdlog::info("Logging initialized with spdlog.");

    const auto error = bf::makeDxError("CreateDevice", static_cast<bf::HResult>(0x887A0005L), "while probing adapters");
    spdlog::error("{}", bf::formatError(error));

    bf::log::shutdown();
    return 0;
}
