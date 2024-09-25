/**
 * @file paths.hpp
 *
 * @brief Get file paths.
 */

#pragma once

#include <filesystem>  // for std::filesystem
#include <stdexcept>   // for std::runtime_error
#include <string>      // for std::string

namespace core::paths {

/**
 * @brief Get the absolute path to the user-specific resources directory.
 *
 * This retrieves the platform-specific resources directory for a given application.
 *
 * On macOS, it stores resources in "~/Library/Application Support/<application_name>/Resources".
 * On GNU/Linux, if the "XDG_DATA_HOME" environment variable is set, resources are stored in the directory specified by this variable. If "XDG_DATA_HOME" is not set, it defaults to "~/.local/share/<application_name>".
 * On Windows, it stores resources in "C:/Users/<username>/AppData/Local/<application_name>/Resources".
 *
 * The "Resources" subdirectory is only appended on Windows and macOS. On GNU/Linux, resources are stored directly in the "XDG_DATA_HOME" directory or its default location without an additional "Resources" subdirectory.
 *
 * @param application_name Name of the application (e.g., "MyApp").
 * @param create_directories If true, create the directory if it does not exist (default: true).
 *
 * @return Absolute path to the resources directory (e.g., "~/Library/Application Support/MyApp/Resources").
 *
 * @throws std::runtime_error If retrieval or creation fails.
 */
[[nodiscard]] std::filesystem::path get_resources_directory(const std::filesystem::path &application_name,
                                                            const bool create_directories = true);

}  // namespace core::paths
