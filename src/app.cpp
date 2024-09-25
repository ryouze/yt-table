/**
 * @file app.cpp
 */

#include <iostream>  // for std::cin
#include <string>    // for std::string, std::getline
#include <vector>    // for std::vector

#include <fmt/core.h>

#include "app.hpp"
#include "core/args.hpp"
#include "core/io.hpp"
#include "core/paths.hpp"
#include "core/shell.hpp"
#include "modules/disk.hpp"
#include "version.hpp"

namespace app {

namespace {

/**
 * @brief Private helper function to print the names of the channels.
 *
 * The function will first print a leading newline, then the number of channels, and then each channel's name, link, description, and a trailing newline.
 *
 * @param channels Vector of YouTube channels.
 */
void print_channel_names(const std::vector<core::io::Channel> &channels)
{
    fmt::print("\nChannels ({}):\n", channels.size());
    for (const auto &channel : channels) {
        fmt::print("  Name: {}\n"
                   "  Link: {}\n"
                   "  Description: {}\n\n",
                   channel.name, channel.link, channel.description);
    }
    // If empty, print a newline, otherwise, the last channel will have a trailing newline
    if (channels.empty()) {
        fmt::print("\n");
    }
}

/**
 * @brief Get user input from the console.
 *
 * @param prompt Prompt to display before the input (e.g., "Name: ").
 *
 * @return String containing the user input.
 */
[[nodiscard]] std::string get_input(const std::string &prompt)
{
    std::string input;
    do {
        fmt::print("{}", prompt);
        std::getline(std::cin, input);
    } while (input.empty());
    return input;
}

}  // namespace

void run()
{
    // Load the HTML table from disk
    modules::disk::Table table(core::paths::get_resources_directory("yt-table") / "subscriptions.html");

    // Print the path to the loaded table
    fmt::print("Loaded: {}\n", table.get_filepath().string());

    // Define the prompt
    const std::string prompt = "[yt-table] $ ";

    // Print the list of channels before the main loop
    print_channel_names(table.get_channels());

    // Start main shell-like loop
    while (true) {
        // Get user input using the UNIX-like prompt
        const std::string input = get_input(prompt);

        // Break the loop
        if (input == "exit") {
            break;
        }
        // Show the help message
        else if (input == "help") {
            fmt::print("Commands:\n"
                       "  help     print this help message\n"
                       "  version  print the version\n"
                       "  ls       print the list of channels\n"
                       "  open     open the html table in a web browser\n"
                       "  add      add a new channel (name, description, link)\n"
                       "  remove   remove a channel (name)\n"
                       "  exit     exit the program\n");
        }
        else if (input == "version") {
            fmt::print("yt-table {}\n", PROJECT_VERSION);
        }
        // Display the list of channels
        else if (input == "ls") {
            print_channel_names(table.get_channels());
        }
        // Open the HTML table in a web browser
        else if (input == "open") {
            fmt::print("Opening: {}\n", table.get_filepath().string());
            core::shell::open_web_browser(table.get_filepath());
        }
        // Add a new channel
        else if (input == "add") {
            const std::string name = get_input("Enter name: ");
            const std::string description = get_input("Enter description: ");
            const std::string link = get_input("Enter link: ");

            table.add(core::io::Channel{name, link, description});

            fmt::print("Channel '{}' added\n", name);
        }
        // Remove a channel
        else if (input == "remove") {
            std::string name = get_input("Enter name: ");

            // If the channel was found, remove it
            if (table.remove(name)) {
                fmt::print("Channel '{}' removed\n", name);
            }
            else {
                fmt::print("Channel '{}' not found\n", name);
            }
        }
        // Unknown command
        else {
            fmt::print("Unknown command: {}\n", input);
        }
    }
}

}  // namespace app
