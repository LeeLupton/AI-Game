#include "Blockforge/Core/Error.h"

#include <fmt/format.h>

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace bf {

std::string formatError(const Error& error)
{
    return fmt::format("{}|{}|{}|{}", error.category, error.code, error.message, error.context);
}

std::string describeHResult(HResult hr)
{
#ifdef _WIN32
    LPWSTR buffer = nullptr;
    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    const DWORD langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
    const DWORD length = FormatMessageW(flags, nullptr, static_cast<DWORD>(hr), langId, reinterpret_cast<LPWSTR>(&buffer), 0, nullptr);

    std::string message;
    if (length != 0 && buffer != nullptr) {
        const int utf8Length = WideCharToMultiByte(CP_UTF8, 0, buffer, length, nullptr, 0, nullptr, nullptr);
        if (utf8Length > 0) {
            std::string converted(static_cast<std::size_t>(utf8Length), '\0');
            WideCharToMultiByte(CP_UTF8, 0, buffer, length, converted.data(), utf8Length, nullptr, nullptr);
            while (!converted.empty() && (converted.back() == '\r' || converted.back() == '\n')) {
                converted.pop_back();
            }
            message = fmt::format("0x{:08X}: {}", static_cast<std::uint32_t>(hr), converted);
        }
    }

    if (buffer != nullptr) {
        LocalFree(buffer);
    }

    if (!message.empty()) {
        return message;
    }

    return fmt::format("0x{:08X}: Unknown HRESULT", static_cast<std::uint32_t>(hr));
#else
    return fmt::format("0x{:08X}: HRESULT translation requires Windows runtime", static_cast<std::uint32_t>(hr));
#endif
}

Error makeDxError(std::string code, HResult hr, std::string_view context)
{
    return Error{
        .category = "DX12",
        .code = std::move(code),
        .message = describeHResult(hr),
        .context = std::string(context),
    };
}

}  // namespace bf
