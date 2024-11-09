/**
 * @file strings.cpp
 */

#include <string>  // for std::string

#include "strings.hpp"

namespace core::strings {

std::string trim_whitespace(const std::string &str)
{
    // Find the first non-whitespace character
    const auto first = str.find_first_not_of(" \t\n\r");

    // If the string is empty, return an empty string
    if (first == std::string::npos) {
        return "";
    }

    // Find the last non-whitespace character
    const auto last = str.find_last_not_of(" \t\n\r");

    // Return the trimmed string
    return str.substr(first, last - first + 1);
}

}  // namespace core::strings
