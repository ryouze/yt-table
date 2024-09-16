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
 * @brief Base class for exceptions raised by the shell.
 */
class ShellError : public std::runtime_error {
  public:
    explicit ShellError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Base class for exceptions raised by the command builder.
 */
class PathError : public std::runtime_error {
  public:
    explicit PathError(const std::string &message)
        : std::runtime_error(message) {}
};

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
 * @return Command to open the default web browser (e.g., `open "~/data.html"`).
 *
 * @throws PathError If failed to build the command or the platform is not supported.
 */
[[nodiscard]] std::string build_command(const std::string &filepath);

/**
 * @brief Run a shell command.
 *
 * @param command Shell command to run (e.g., "ls -l").
 *
 * @throws ShellError if failed to run the command.
 *
 * @note This function is very dangerous and should be used with caution. In this program, it is used to open the HTML table in the default web browser.
 *
 * @details Since the web browser command is not a major function of the application, I decided to use the standard library to run shell commands. If the application were to have more shell commands (especially user-provided ones), I would consider using a safer, OS-specific API. Otherwise, I'd rather avoid them, unless absolutely necessary, as seen with my "pathmaster" library.
 */
void run(const std::string &command);

/**
 * @brief Open the default web browser with the specified path or URL.
 *
 * The risk of shell injection attacks is minimal because the filepath is provided by the "pathmaster" library, which was also created by me (see "app.cpp").
 *
 * @param filepath Path to the file to open in the web browser (e.g., "~/data.html").
 *
 * @note Under the hood, this runs a platform-specific shell command to open the web browser.
 *
 * @details Since the web browser command is not a major function of the application, I decided to use the standard library to run shell commands. If the application were to have more shell commands (especially user-provided ones), I would consider using a safer, OS-specific API. Otherwise, I'd rather avoid them, unless absolutely necessary, as seen with my "pathmaster" library.
 */
void open_web_browser(const std::string &filepath);

}  // namespace core::shell
