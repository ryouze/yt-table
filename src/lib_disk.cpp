#include "lib_disk.hpp"  // lib_disk::HTMLFile
#include <cstddef>       // std::size_t
#include <filesystem>    // [C++17 required] std::filesystem::remove, std::filesystem::rename, std::filesystem::is_empty
#include <fstream>       // std::fstream, std::ofstream
#include <ios>           // std::ios_base::in, std::ios_base::out
#include <map>           // std::map
#include <sstream>       // std::stringstream
#include <stdexcept>     // std::runtime_error
#include <string>        // std::string, std::getline
#include <vector>        // std::vector

namespace lib_disk {

/**
 * @brief Get HTML template in parts: beginning and end.
 * @details If you want to modify the HTML's appearance, edit the `<style>` tag in `get_beginning()`.
 */
namespace html_template {

/**
 * @brief Get the beginning of HTML - doctype, HTML opening tag, head, style, body opening tag, main opening tag, table with header.
 * @details Indentation and trailing newline are included.
 * @return The beginning of a HTML file, with table opening tag and table header.
 */
[[nodiscard]] std::string get_beginning()
{
    return R"(<!DOCTYPE html>
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
      border-radius: 25px;
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
}

/**
 * @brief Get the ending of HTML - main closing tag, body closing tag, HTML closing tag.
 * @details Indentation and trailing newline are included.
 * @return The end of a HTML file, with table closing tag.
 */
[[nodiscard]] std::string get_ending()
{
    return R"(    </table>
  </main>
</body>

</html>
)";
}

}  // namespace html_template

HTMLFile::HTMLFile(const std::string &filepath) : AbstractFile(filepath)  // set private variable `this->filepath_` (reference to `std::string`)
{
    // prepare for read/write mode
    std::fstream f;
    // if file doesn't exist or empty, write placeholder to disk
    // this should only be triggered when the program is ran for the first time, so accessing the file twice is a non-issue
    if (!std::filesystem::exists(this->filepath_) || std::filesystem::is_empty(this->filepath_)) {
        f.open(this->filepath_, std::ios_base::out);  // open in write mode
        if (!f) {
            throw std::runtime_error("Cannot write to HTML file '" + this->filepath_ + "' to create placeholder, possibly due to missing permissions.");
        }
        // write html placeholder to disk
        f << html_template::get_beginning() << html_template::get_ending();
        // close() & clear() are mandatory when re-using `std::fstream`
        f.close();
        f.clear();  // clear flags
    }
    /*
    For each line that is not empty and contains data cell (`<td>`), toggle boolean `is_link` between true & false under the naive assumption that:
    * 1) First data cell is always the link.
    * 2) Second data cell is always the description.

    This will break completely if two `<td>`'s are on the same line, which is, nevertheless, perfectly valid HTML.
    Simply put, we're naively assuming that the data cells will always be newline-separated, as that's how the program formats the output HTML - it creates newline-separated data cells and it reads newline-separated data cells.

    Leading & trailing whitespace are not removed, because we expect to store raw HTML data internally.
    */
    f.open(this->filepath_, std::ios_base::in);  // open in read mode
    std::string current_line;
    std::string previous_line;
    bool is_link = true;
    while (std::getline(f, current_line)) {
        if (!current_line.empty() && current_line.find("<td>") != std::string::npos) {
            if (is_link) {
                previous_line = current_line;
                is_link = false;
            }
            else {
                // append to private map
                this->subscriptions_.insert({previous_line, current_line});
                is_link = true;
            }
        }
    }
}

[[nodiscard]] std::string HTMLFile::get_status() const
{
    std::string res = "* this->filepath='" + this->filepath_ + "'\n";
    for (const auto &itr : this->subscriptions_) {
        res += "* link={" + itr.first + "}; description={" + itr.second + "}\n";
    }
    return res;
}

void HTMLFile::add(const std::string &string_to_split)
{
    // split string at semicolons into three items
    std::vector<std::string> elements;
    {
        std::string buffer;
        // remove leading & trailing whitespace from the entire string
        std::stringstream s(this->remove_whitespace(string_to_split));
        while (std::getline(s, buffer, ';')) {
            // remove leading & trailing whitespace from each individual item
            elements.push_back(this->remove_whitespace(buffer));
        }
        // we need exactly three items: `NAME;DESCRIPTION;LINK`
        const std::size_t elements_count = elements.size();
        if (elements_count != 3) {
            throw std::runtime_error("Could not extract three items (`NAME;DESCRIPTION;LINK`) from string '" + string_to_split + "' (" + std::to_string(elements_count) + " items were extracted). Use `--help` to display examples.");
        }
    }
    // extract all three elements from vector
    const std::string name = elements.at(0);
    const std::string description = elements.at(1);
    const std::string link = elements.at(2);
    /*
    1. Check if subscription doesn't already exist.

    2. Append both `<td>` tags to private map, where:
    * 1) `first` is the full link, as constructed from `link` & `name`.
    * 2) `second` is the description, as constructed from `description`.
    */
    const std::string key = R"(        <td><a target="_blank" href=")" + link + R"(">)" + name + "</a></td>";
    if (this->subscriptions_.count(key) != 0) {
        throw std::runtime_error("Cannot add channel '" + name + "', because it already exists: '" + this->remove_whitespace(key) + "'.");
    }
    this->subscriptions_.insert(
        {key, "        <td>" + description + "</td>"});
}

void HTMLFile::remove(const std::string &name)
{
    if (this->subscriptions_.empty()) {
        throw std::runtime_error("Cannot remove channel '" + name + "', because the subscriptions list doesn't contain any channels.");
    }
    // remove whitespace, naively append the display part of the `<a>` tag because I had issues with regex
    const std::string name_extended = '>' + this->remove_whitespace(name) + "</a></td>";
    for (const auto &itr : this->subscriptions_) {
        if (itr.first.find(name_extended) != std::string::npos) {
            // remove key from private map
            this->subscriptions_.erase(itr.first);
            return;
        }
    }
    throw std::runtime_error("Cannot remove channel '" + name + "', because it is not present in the subscriptions list.");
}

void HTMLFile::write_to_disk() const
{
    // create backup of current filepath by renaming it to current filepath + ".bak" (old backup will be deleted)
    this->force_rename(this->filepath_ + ".bak");
    std::ofstream f(this->filepath_);
    if (!f) {
        throw std::runtime_error("Cannot write to HTML file '" + this->filepath_ + "' to save results, possibly due to missing permissions.");
    }
    // write HTML file to disk
    f << html_template::get_beginning();  // add opening (head, body, table opening tag)
    for (const auto &itr : this->subscriptions_) {
        // add each pair as two consecutive lines of within `<tr>` tags
        f << "      <tr>\n"
          << itr.first << '\n'
          << itr.second << '\n'
          << "      </tr>\n";
    }
    f << html_template::get_ending();  // add closing (table, body closing tags)
}

inline void HTMLFile::force_rename(const std::string &new_filepath) const
{
    std::filesystem::remove(new_filepath);                   // True if the file was deleted, False if it did not exist. If failed, throw.
    std::filesystem::rename(this->filepath_, new_filepath);  // If failed, throw.
}

[[nodiscard]] std::string HTMLFile::remove_whitespace(const std::string &str, const std::string &whitespace)
{
    const size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        throw std::runtime_error("Cannot remove trailing whitespace from '" + str + "', because it's empty.");
    }
    const size_t end = str.find_last_not_of(whitespace);
    const size_t range = end - start + 1;
    return str.substr(start, range);
}

}  // namespace lib_disk
