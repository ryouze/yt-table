/**
 * @file io.hpp
 *
 * @brief Input/output functions.
 */

#pragma once

namespace core::io {

/**
 * @brief Setup UTF-8 input/output on Windows. Do nothing on other platforms.
 *
 * @throws std::runtime_error If failed to enable UTF-8 encoding on Windows.
 */
void setup_utf8_console();

}  // namespace core::io
