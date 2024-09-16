/**
 * @file shell.cpp
 */

#include <cstdlib>    // for std::system
#include <exception>  // for std::exception
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

#include <fmt/core.h>

#include "shell.hpp"

std::string core::shell::build_command(const std::string &filepath)
{
    try {
        // Get the command to open the default web browser based on the platform
        std::string open_command;
#if defined(__APPLE__)
        open_command = "open";
#elif defined(__linux__)
        open_command = "xdg-open";
#elif defined(_WIN32)
        // Ensure no new command window opens by providing an empty title
        open_command = "start \"\"";
#else
        throw std::runtime_error("Unsupported platform");
#endif

        // Return command with filepath wrapped in quotes to prevent issues with spaces
        return fmt::format("{} \"{}\"", open_command, filepath);
    }
    catch (const std::exception &e) {
        throw core::shell::PathError(fmt::format("Failed to build command: {}", e.what()));
    }
}

void core::shell::run(const std::string &command)
{
    // Execute the command and check for non-zero exit status
    if (std::system(command.c_str()) != 0) {
        throw core::shell::ShellError(fmt::format("Failed to run the command: {}", command));
    }
}

void core::shell::open_web_browser(const std::string &filepath)
{
    // Run the command (this will open the web browser with the HTML table)
    core::shell::run(core::shell::build_command(filepath));
}
