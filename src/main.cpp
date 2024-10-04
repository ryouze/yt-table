/**
 * @file main.cpp
 */

#include <cstdlib>    // for EXIT_FAILURE, EXIT_SUCCESS
#include <exception>  // for std::exception

#include <fmt/core.h>

#include "app.hpp"
#include "core/args.hpp"
#if defined(_WIN32)
#include "core/io.hpp"
#endif

/**
 * @brief Entry-point of the application.
 *
 * @param argc Number of command-line arguments (e.g., "2").
 * @param argv Array of command-line arguments (e.g., {"./bin", "-h"}).
 *
 * @return EXIT_SUCCESS if the application ran successfully, EXIT_FAILURE otherwise.
 */
int main(int argc,
         char **argv)
{
    try {
#if defined(_WIN32)
        // Setup UTF-8 input/output on Windows (does nothing on other platforms)
        if (const auto e = core::io::setup_utf8_console(); e.has_value()) {
            fmt::print(stderr, "Warning: {}\n", *e);
        }
#endif

        // Parse command-line arguments, but do not pass them, as this class only checks for "--help" or "--version"
        core::args::Args(argc, argv);

        // Run the application
        app::run();
    }
    catch (const core::args::ArgsMessage &e) {
        // User requested help or version
        fmt::print("{}\n", e.what());
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "{}\n", e.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        fmt::print(stderr, "Error: Unknown\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
