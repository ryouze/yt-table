/**
 * @file shell.hpp
 *
 * @brief Run shell commands.
 */

#pragma once

#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace core::shell {

/**
 * @brief Build the command to open the filepath in the default web browser (or any other default application for a given file type).
 *
 * The following commands are returned:
 * - macOS: "open"
 * - GNU/Linux: "xdg-open"
 * - Windows: "start"
 *
 * The filepath is wrapped in quotes before being concatenated with the platform-specific command.
 *
 * @return Command to open the default web browser (e.g., open "~/data.html").
 *
 * @throws std::runtime_error If failed to build the command or the platform is not supported.
 */
[[nodiscard]] std::string build_command(const std::string &filepath);

/**
 * @brief Run a shell command.
 *
 * @param command Shell command to run (e.g., "ls -l").
 *
 * @throws std::runtime_error if failed to run the command.
 *
 * @note This function is very dangerous and should be used with caution. In this program, it is used to open the HTML table in the default web browser.
 *
 * @details Since the web browser command is not a major function of the application, I decided to use the standard library to run shell commands. If the application were to have more shell commands (especially user-provided ones), I would consider using a safer, OS-specific API. Otherwise, I'd rather avoid them, unless absolutely necessary.
 */
void run(const std::string &command);

/**
 * @brief Open the default web browser with the specified path or URL.
 *
 * The risk of shell injection attacks is minimal because the filepath is created by me using OS-specific APIs (see "paths.cpp").
 *
 * @param filepath Path to the file to open in the web browser (e.g., "~/data.html").
 *
 * @note Under the hood, this runs a platform-specific shell command to open the web browser.
 */
void open_web_browser(const std::string &filepath);

}  // namespace core::shell
