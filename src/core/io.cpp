/**
 * @file io.cpp
 */

#include <stdexcept>  // for std::runtime_error
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <locale>            // for setlocale, LC_ALL
#include <windows.h>         // for WideCharToMultiByte, GetLastError, CP_UTF8, SetConsoleCP, SetConsoleOutputCP
#endif

#include <fmt/core.h>

#include "io.hpp"

namespace core::io {

void setup_utf8_console()
{
#if defined(_WIN32)
    if (!SetConsoleCP(CP_UTF8) || !SetConsoleOutputCP(CP_UTF8)) {
        throw std::runtime_error(fmt::format("Failed to set UTF-8 code page: {}", GetLastError()));
    }

    if (!setlocale(LC_ALL, ".UTF8")) {
        throw std::runtime_error("Failed to set UTF-8 locale");
    }
#endif
}

}  // namespace core::io
