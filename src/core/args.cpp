/**
 * @file args.cpp
 */

#include <string>  // for std::string

#include <fmt/core.h>

#include "args.hpp"
#include "version.hpp"

core::args::Args::Args(const int argc,
                       char **argv)
{
    // If no arguments, do nothing
    if (argc == 1) {
        return;
    }
    // If one or more arguments
    else {
        // Define the formatted help message
        const std::string help_message =
            "Usage: yt-table [-h] [-v]\n"
            "\n"
            "Manage YouTube subscriptions locally through a shell-like interface.\n"
            "\n"
            "Optional arguments:\n"
            "  -h, --help     prints help message and exits\n"
            "  -v, --version  prints version and exits\n";

        // Get the first argument as a string
        const std::string arg = argv[1];

        if (arg == "-h" || arg == "--help") {
            // If "-h" or "--help" is passed as the first argument, throw ArgsMessage with the help message
            throw ArgsMessage(help_message);
        }
        else if (arg == "-v" || arg == "--version") {
            // If "-v" or "--version" is passed as the first argument, throw ArgsMessage with the version
            throw ArgsMessage(fmt::format("{}", PROJECT_VERSION));
        }
        else {
            // Otherwise, throw ArgsError with the help message
            throw ArgsError(fmt::format("Error: Invalid argument: {}\n\n{}", arg, help_message));
        }
    }
}
