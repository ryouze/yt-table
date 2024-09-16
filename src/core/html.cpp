/**
 * @file html.cpp
 */

#include <algorithm>    // for std::sort
#include <exception>    // for std::exception
#include <filesystem>   // for std::filesystem
#include <fstream>      // for std::ifstream
#include <regex>        // for std::regex, std::smatch, std::sregex_iterator
#include <sstream>      // for std::ostringstream
#include <stdexcept>    // for std::runtime_error
#include <string>       // for std::string
#include <string_view>  // for std::string_view
#include <vector>       // for std::vector

#include <fmt/core.h>
#include <fmt/os.h>

#include "html.hpp"

namespace {

/**
 * @brief Private helper variable that contains the start of the HTML file.
 *
 * @note This must be followed by rows.
 */
constexpr std::string_view html_template_start = R"(<!DOCTYPE html>
<html lang="en">

  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Subscriptions</title>
    <style>
      body {
        background-color: black;
        border: none;
        color: #d3d3d3;
        font-family: Arial, Helvetica, sans-serif;
        height: 100%;
        margin-top: 2rem;
        margin-bottom: 2rem;
        overflow-x: hidden;
        overflow-y: scroll;
        text-align: center;
      }

      * {
        margin: 0;
        padding: 0;
      }

      a {
        color: #ff6961;
        text-decoration: none;
      }

      a:hover {
        color: #ff9eb5;
      }

      main {
        display: block;
        margin: auto;
        max-width: 600px;
      }

      main>table {
        background-color: #0d0d0d;
        border-radius: 15px;
        border-spacing: 2em;
        border: 2px solid #262626;
        table-layout: fixed;
        width: 100%;
      }

      main>table tr>th {
        color: #bfbfbf;
        font-size: 130%;
        font-weight: bold;
      }

      main>table tr>td {
        color: #828282;
        overflow-wrap: anywhere;
      }
    </style>
  </head>

  <body>
    <main>
      <table>
        <tr>
          <th>Name</th>
          <th>Desc<wbr>ription</th>
        </tr>
)";

/**
 * @brief Private helper variable that contains the end of the HTML file.
 *
 * @note This must be preceded by rows.
 */
constexpr std::string_view html_template_end = R"(      </table>
    </main>
  </body>

</html>
)";

}  // namespace

std::vector<core::html::Channel> core::html::load(const std::string &input_path,
                                                  const bool create_backup)
{
    // Error: Doesn't exist
    if (!std::filesystem::exists(input_path)) {
        throw core::html::FileNotFoundError(input_path);
    }
    // Otherwise, load the file
    try {
        // Backup to prevent data loss
        if (create_backup) {
            std::filesystem::copy_file(
                input_path,                                        // Source
                input_path + ".bak",                               // Destination
                std::filesystem::copy_options::overwrite_existing  // Overwrite if exists
            );
        }

        // Initialize a string to store the file contents
        std::string text;

        {
            // Open the file in read mode
            std::ifstream file(input_path);

            // Error: File cannot be opened
            if (!file) {
                throw std::runtime_error(fmt::format("Failed to open file '{}' for reading", input_path));
            }

            // Read the file contents into a string
            std::ostringstream buffer;
            buffer << file.rdbuf();
            text = buffer.str();
        }  // Close the file and deallocate buffer, keeping only the text in memory

        // Initialize a vector of channels
        std::vector<core::html::Channel> channels;

        // Define a regex pattern to match the HTML structure
        static const std::regex pattern(
            R"(<tr>\s*<td><a\s+[^>]*href=\"([^\"]+)\"[^>]*>([^<]+)</a></td>\s*<td>([^<]+)</td>\s*</tr>)",
            std::regex::icase | std::regex::optimize);

        // Iterate over the matches
        std::sregex_iterator it(text.cbegin(), text.cend(), pattern);
        std::sregex_iterator end;
        while (it != end) {
            std::smatch match = *it;
            // If the match size is 4, we have a valid match
            if (match.size() == 4) {
                // match[0] is the whole match, we need match[1], match[2], and match[3]
                const std::string link = match[1].str();
                const std::string name = match[2].str();
                const std::string description = match[3].str();

                channels.emplace_back(name, link, description);
            }
            else {
                // If the match size is not 4, throw an error
                throw core::html::HtmlParseError(fmt::format(
                    "Unexpected match size '{}' (expected 4) in: {}",
                    match.size(), match.str()));
            }
            ++it;
        }

        // Sort channels by name
        std::sort(channels.begin(), channels.end(),
                  [](const core::html::Channel &a, const core::html::Channel &b) {
                      return a.name < b.name;
                  });

        // Return shrunk vector (RVO)
        channels.shrink_to_fit();
        return channels;
    }
    catch (const core::html::HtmlParseError &) {
        throw;  // Re-throw HtmlParseError as-is
    }
    catch (const std::exception &e) {
        throw core::html::IOError(fmt::format(
            "Error processing file '{}': {}",
            input_path, e.what()));
    }
}

void core::html::save(const std::string &output_path,
                      const std::vector<core::html::Channel> &channels)
{
    try {
        // Open the file for writing using fmt::output_file
        auto file = fmt::output_file(output_path);

        // Write the start of the HTML template
        file.print("{}", html_template_start);

        // Write each channel's HTML row
        for (const auto &channel : channels) {
            file.print(
                "        <tr>\n"
                "          <td><a target=\"_blank\" href=\"{}\">{}</a></td>\n"
                "          <td>{}</td>\n"
                "        </tr>\n",
                channel.link, channel.name, channel.description);
        }

        // Write the end of the HTML template
        file.print("{}", html_template_end);
    }
    catch (const std::exception &e) {
        throw core::html::IOError(fmt::format(
            "Failed to save file '{}': {}",
            output_path, e.what()));
    }
}
