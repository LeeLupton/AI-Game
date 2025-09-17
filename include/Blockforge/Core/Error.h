#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace bf {

struct Error {
    std::string category;
    std::string code;
    std::string message;
    std::string context;
};

//! Formats the error according to the canonical template used throughout the engine.
std::string formatError(const Error& error);

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
using HResult = HRESULT;
#else
using HResult = std::int32_t;
#endif

//! Translates an HRESULT value to a human readable message.
std::string describeHResult(HResult hr);

//! Creates a DirectX 12 themed error using the canonical format.
Error makeDxError(std::string code, HResult hr, std::string_view context);

}  // namespace bf
