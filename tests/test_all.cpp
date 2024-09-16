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
#include <pathmaster/pathmaster.hpp>

#include "app.hpp"
#include "core/args.hpp"
#include "core/html.hpp"
#include "core/shell.hpp"
#include "modules/disk.hpp"

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
 * @return EXIT_SUCCESS if the application ran successfully, EXIT_FAILURE otherwise.
 */
int main(int argc,
         char **argv)
{
#if defined(_WIN32)
    // Enable UTF-8 output on Windows
    pathmaster::setup_utf8_console_output();
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

    // Get the first argument as a string
    const std::string arg = argv[1];

    // If found in the map, run the test
    if (const auto it = tests.find(arg); it != tests.cend()) {
        return it->second();
    }
    else if (arg == "all") {
        // Run all tests sequentially and print the results
        bool all_passed = true;
        for (const auto &[name, test_func] : tests) {
            fmt::print("Running test: {}\n", name);
            const int result = test_func();
            if (result != EXIT_SUCCESS) {
                all_passed = false;
                fmt::print(stderr, "Test '{}' failed.\n", name);
            }
            else {
                fmt::print("Test '{}' passed.\n", name);
            }
        }
        return all_passed ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else {
        fmt::print("Error: Invalid argument: {}\n\n{}\n", arg, help_message);
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
        const char *fake_argv[] = {TEST_EXECUTABLE_NAME, "-h"};
        core::args::Args(2, const_cast<char **>(fake_argv));
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: no help message displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsMessage &) {
        fmt::print("core::args::Args() passed: help message displayed.\n");
        return EXIT_SUCCESS;
    }
}

int test_args::version()
{
    try {
        const char *fake_argv[] = {TEST_EXECUTABLE_NAME, "-v"};
        core::args::Args(2, const_cast<char **>(fake_argv));
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: no version displayed.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsMessage &e) {
        fmt::print("core::args::Args() passed: version displayed: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_args::invalid()
{
    try {
        const char *fake_argv[] = {TEST_EXECUTABLE_NAME, "hello"};
        core::args::Args(2, const_cast<char **>(fake_argv));
        // This should never be reached, as the ArgsError exception should be thrown by the constructor
        fmt::print("core::args::Args() failed: invalid argument wasn't caught.\n");
        return EXIT_FAILURE;
    }
    catch (const core::args::ArgsError &e) {
        fmt::print("core::args::Args() passed: invalid argument caught: {}\n", e.what());
        return EXIT_SUCCESS;
    }
}

int test_html::save_load()
{
    try {
        // Get path to the resources directory
        const auto temp_file = (pathmaster::get_resources_directory(TEST_EXECUTABLE_NAME) / "test_channels.html").string();

        // Create a dummy vector of channels ("core::html::load" will sort them alphabetically by name, so the order is important!)
        const std::vector<core::html::Channel> channels = {
            core::html::Channel("Engineering Explained", "https://www.youtube.com/@EngineeringExplained", "Car Engineering"),
            core::html::Channel("Noriyaro", "https://www.youtube.com/@noriyaro/videos", "JP Drifting"),
            core::html::Channel("チャンネル", "https://www.youtube.com/@channel/videos", "日本語"),  // Japanese characters
        };

        // Save the channels to the temporary file
        core::html::save(temp_file, channels);
        fmt::print("core::html::save() passed: saved to {}.\n", temp_file);

        // Load the channels back from the temporary file for comparison
        const auto loaded_channels = core::html::load(temp_file);
        fmt::print("core::html::load() passed: loaded from {}.\n", temp_file);

        // Verify the loaded channels match the original
        if (loaded_channels != channels) {
            throw std::runtime_error("Loaded channels do not match the original");
        }

        fmt::print("core::html::load() passed: loaded channels match the original.\n");

        // Remove directory and its contents
        std::filesystem::remove_all(std::filesystem::path(temp_file).parent_path());
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "core::html::save/load() failed: {}\n", e.what());
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
        const auto temp_file = (pathmaster::get_resources_directory(TEST_EXECUTABLE_NAME) / "test_table.html").string();

        // Create a table at the temporary file path
        modules::disk::Table table(temp_file);
        fmt::print("modules::disk::Table() passed: created table at {}.\n", temp_file);

        // Add a channel to the table
        table.add(core::html::Channel("Noriyaro", "https://www.youtube.com/@noriyaro/videos", "JP Drifting"));
        fmt::print("modules::disk::Table::add() passed: added channel to the table.\n");

        // Remove the channel from the table
        if (!table.remove("Noriyaro")) {
            throw std::runtime_error("Failed to remove the channel from the table");
        }
        fmt::print("modules::disk::Table::remove() passed: removed channel from the table.\n");

        // Remove directory and its contents
        std::filesystem::remove_all(std::filesystem::path(temp_file).parent_path());
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "modules::disk::Table() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}

int test_app::help()
{
    try {
        const char *fake_argv[] = {TEST_EXECUTABLE_NAME, "-h"};
        core::args::Args(2, const_cast<char **>(fake_argv));
        // This should never be reached, as the ArgsError exception should be thrown
        throw std::runtime_error("Did not exit immediately after being asked to display the help message with '-h'");
    }
    catch (const core::args::ArgsMessage &) {
        fmt::print("app::run() passed.\n");
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        fmt::print(stderr, "app::run() failed: {}\n", e.what());
        return EXIT_FAILURE;
    }
}
