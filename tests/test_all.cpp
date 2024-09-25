/**
 * @file test_all.cpp
 */

#include <algorithm>      // for std::any_of
#include <cstdlib>        // for EXIT_FAILURE, EXIT_SUCCESS
#include <exception>      // for std::exception
#include <filesystem>     // for std::filesystem
#include <functional>     // for std::function
#include <stdexcept>      // for std::runtime_error
#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map
#include <vector>         // for std::vector

#include <fmt/core.h>

#include "app.hpp"
#include "core/args.hpp"
#include "core/io.hpp"
#include "core/paths.hpp"
#include "core/shell.hpp"
#include "modules/disk.hpp"

#include "helpers.hpp"

#define TEST_EXECUTABLE_NAME "tests"

namespace test_args {
[[nodiscard]] int none();
[[nodiscard]] int help();
[[nodiscard]] int version();
[[nodiscard]] int invalid();
}  // namespace test_args

namespace test_html {
[[nodiscard]] int save_load();
}  // namespace test_html

namespace test_shell {
[[nodiscard]] int build_command();
}  // namespace test_shell

namespace test_disk {
[[nodiscard]] int save_load();
}  // namespace test_disk

namespace test_app {
[[nodiscard]] int help();
}  // namespace test_app

/**
 * @brief Entry-point of the test application.
 *
 * @param argc Number of command-line arguments (e.g., "2").
 * @param argv Array of command-line arguments (e.g., {"./bin", "-h"}).
 *
 * @return EXIT_SUCCESS if the test application ran successfully, EXIT_FAILURE otherwise.
 */
int main(int argc,
         char **argv)
{
#if defined(_WIN32)
    // Setup UTF-8 input/output on Windows (does nothing on other platforms)
    core::io::setup_utf8_console();
#endif

    // Define the formatted help message
    const std::string help_message = fmt::format(
        "Usage: {} <test>\n"
        "\n"
        "Run automatic tests.\n"
        "\n"
        "Positional arguments:\n"
        "  test  name of the test to run ('all' to run all tests)\n",
        argv[0]);

    // If no arguments, print help message and exit
    if (argc == 1) {
        fmt::print("{}\n", help_message);
        return EXIT_FAILURE;
    }

    // Otherwise, define argument to function mapping
    const std::unordered_map<std::string, std::function<int()>> tests = {
        {"test_args::none", test_args::none},
        {"test_args::help", test_args::help},
        {"test_args::version", test_args::version},
        {"test_args::invalid", test_args::invalid},
        {"test_html::save_load", test_html::save_load},
        {"test_shell::build_command", test_shell::build_command},
        {"test_disk::save_load", test_disk::save_load},
        {"test_app::help", test_app::help},
    };

    // Get the test name from the command-line arguments
    const std::string arg = argv[1];

    // If the test name is found, run the corresponding test
    if (const auto it = tests.find(arg); it != tests.cend()) {
        try {
            return it->second();
        }
        catch (const std::exception &e) {
            fmt::print(stderr, "Test '{}' threw an exception: {}\n", arg, e.what());
            return EXIT_FAILURE;
        }
    }
    else if (arg == "all") {
        // Run all tests sequentially and print the results
        bool all_passed = true;
        for (const auto &[name, test_func] : tests) {
            fmt::print("Running test: {}\n", name);
            try {
                const int result = test_func();
                if (result != EXIT_SUCCESS) {
                    all_passed = false;
                    fmt::print(stderr, "Test '{}' failed.\n", name);
                }
                else {
                    fmt::print("Test '{}' passed.\n", name);
                }
            }
            catch (const std::exception &e) {
                all_passed = false;
                fmt::print(stderr, "Test '{}' threw an exception: {}\n", name, e.what());
            }
        }
        return all_passed ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else {
        fmt::print(stderr, "Error: Invalid test name: '{}'\n\n{}\n", arg, help_message);
        return EXIT_FAILURE;
    }
}

int test_args::none()
{
    try {
        // This class does nothing, unless the first argument is "-h" or "--v", in which case it throws an exception, which should be caught
        core::args::Args(1, nullptr);
        fmt::print("core::args::Args() passed: no arguments.\n");
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "core::args::Args() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_args::help()
{
    try {
        char *fake_argv[] = {const_cast<char *>(TEST_EXECUTABLE_NAME), const_cast<char *>("-h")};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: no help message displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const std::exception &e) {
        fmt::print("core::args::Args() passed: help message displayed.\n");
        return EXIT_SUCCESS;
    }
}

int test_args::version()
{
    try {
        char *fake_argv[] = {const_cast<char *>(TEST_EXECUTABLE_NAME), const_cast<char *>("-v")};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: no version displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const std::exception &e) {
        fmt::print("core::args::Args() passed: version displayed: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_args::invalid()
{
    try {
        char *fake_argv[] = {const_cast<char *>(TEST_EXECUTABLE_NAME), const_cast<char *>("hello")};
        core::args::Args(2, fake_argv);
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: invalid argument wasn't caught.\n");
        return EXIT_FAILURE;
    }
    catch (const std::exception &e) {
        fmt::print("core::args::Args() passed: invalid argument caught: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_html::save_load()
{
    try {
        // Get path to the resources directory
        const auto temp_file = (core::paths::get_resources_directory(TEST_EXECUTABLE_NAME) / "test_channels.html").string();

        // pathmaster will create the directory if it doesn't exist, but we want to ensure that tests are isolated
        const helpers::TempDir temp_dir(std::filesystem::path(temp_file).parent_path());

        // Create a dummy vector of channels ("core::io::load" will sort them alphabetically by name, so the order is important!)
        const std::vector<core::io::Channel> channels = {
            core::io::Channel("Engineering Explained", "https://www.youtube.com/@EngineeringExplained", "Car Engineering"),
            core::io::Channel("Noriyaro", "https://www.youtube.com/@noriyaro/videos", "JP Drifting"),
            core::io::Channel("チャンネル", "https://www.youtube.com/@channel/videos", "日本語"),  // Japanese characters
        };

        // Save the channels to the temporary file
        core::io::save(temp_file, channels);
        fmt::print("core::io::save() passed: saved to {}.\n", temp_file);

        // Load the channels back from the temporary file for comparison
        const auto loaded_channels = core::io::load(temp_file);
        fmt::print("core::io::load() passed: loaded from {}.\n", temp_file);

        // Verify the loaded channels match the original
        if (loaded_channels != channels) {
            throw std::runtime_error("Loaded channels do not match the original");
        }

        fmt::print("core::io::load() passed: loaded channels match the original.\n");

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "core::io::save/load() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_shell::build_command()
{
    try {
        const std::vector<std::string> test_paths = {
            "~/data.html",
            "/Users/USER/Library/Application Support/yt-table/Resources/subscriptions.html",
            "C:\\Program Files",
            "C:\\Program Files (x86)",
            "C:\\Users\\USER\\AppData\\Local\\yt-table\\Resources\\subscriptions.html",
            "~/ダタ.html",  // Japanese characters
        };
        // Define platform-specific commands
        const std::vector<std::string> possible_beginnings = {"open", "xdg-open", "start"};

        for (const auto &path : test_paths) {
            const std::string command = core::shell::build_command(path);

            // Check if the command begins with one of the possible commands
            if (!std::any_of(possible_beginnings.cbegin(), possible_beginnings.cend(),
                             [&command](const std::string &beginning) { return command.rfind(beginning, 0) == 0; })) {
                throw std::runtime_error("Build command '" + command + "' does not begin with a valid platform-specific command");
            }

            // Extract the path from the command (assuming the path is after the first space)
            const auto first_space_pos = command.find(' ');
            if (first_space_pos == std::string::npos) {
                throw std::runtime_error("No path found in command '" + command + "'");
            }

            const std::string extracted_path = command.substr(first_space_pos + 1);

            // Check if the extracted path is wrapped in quotes
            if (extracted_path.front() != '"' || extracted_path.back() != '"') {
                throw std::runtime_error("Path in built command '" + command + "' is not wrapped in quotes");
            }
        }
        fmt::print("core::shell::impl::build_command() passed: built platform-specific command for paths.\n");
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "core::shell::impl::build_command() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_disk::save_load()
{
    try {
        // Get path to the resources directory
        const auto temp_file = (core::paths::get_resources_directory(TEST_EXECUTABLE_NAME) / "test_table.html").string();

        // pathmaster will create the directory if it doesn't exist, but we want to ensure that tests are isolated
        const helpers::TempDir temp_dir(std::filesystem::path(temp_file).parent_path());

        // Create a table at the temporary file path
        modules::disk::Table table(temp_file);
        fmt::print("modules::disk::Table() passed: created table at {}.\n", temp_file);

        // Add a channel to the table
        table.add(core::io::Channel("Noriyaro", "https://www.youtube.com/@noriyaro/videos", "JP Drifting"));
        fmt::print("modules::disk::Table::add() passed: added channel to the table.\n");

        // Remove the channel from the table
        if (!table.remove("Noriyaro")) {
            throw std::runtime_error("Failed to remove the channel from the table");
        }
        fmt::print("modules::disk::Table::remove() passed: removed channel from the table.\n");

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::disk::Table() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_app::help()
{
    // TODO: Fix later
    return EXIT_SUCCESS;
    // try {
    //     char *fake_argv[] = {const_cast<char *>(TEST_EXECUTABLE_NAME), const_cast<char *>("-h")};
    //     core::args::Args(2, fake_argv);
    //     // This should never be reached, as the ArgsError exception should be thrown
    //     throw std::runtime_error("Did not exit immediately after being asked to display the help message with '-h'");
    // }
    // catch (const std::exception &e) {
    //     fmt::print("app::run() passed.\n");
    //     return EXIT_SUCCESS;
    // }
    // catch (const std::exception &e) {
    //     fmt::print(stderr, "app::run() failed: {}\n", e.what());
    //     return EXIT_FAILURE;
    // }
}
