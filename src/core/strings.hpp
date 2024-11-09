/**
 * @file strings.hpp
 *
 * @brief Manipulate strings of text.
 */

#pragma once

#include <string>  // for std::string

namespace core::strings {

/**
 * @brief Trim leading and trailing whitespace from a string.
 *
 * @param str String to trim (e.g., "  hello  ").
 *
 * @return Trimmed string (e.g., "hello").
 */
[[nodiscard]] std::string trim_whitespace(const std::string &str);

}  // namespace core::strings
