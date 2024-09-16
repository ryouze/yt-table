/**
 * @file html.hpp
 *
 * @brief Load and save HTML files.
 */

#pragma once

#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string
#include <vector>     // for std::vector

#include <fmt/core.h>

namespace core::html {

/**
 * @brief Base class for exceptions raised during I/O operations.
 */
class IOError : public std::runtime_error {
  public:
    explicit IOError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Raised when a file does not exist.
 */
class FileNotFoundError : public IOError {
  public:
    explicit FileNotFoundError(const std::string &file_path)
        : IOError(fmt::format("FileNotFoundError: The file '{}' does not exist.", file_path)) {}
};

/**
 * @brief Raised when an HTML file cannot be parsed.
 */
class HtmlParseError : public IOError {
  public:
    explicit HtmlParseError(const std::string &file_path)
        : IOError(fmt::format("HtmlParseError: Failed to parse the HTML file '{}'.", file_path)) {}
};

/**
 * @brief Struct that represents a single YouTube channel.
 *
 * @note This struct is marked as `final` to prevent inheritance.
 */
struct Channel final {
    /**
     * @brief Construct a new Channel object.
     *
     * @param _name YouTube Channel's name (e.g., "Noriyaro").
     * @param _link YouTube Channel's link (e.g., "https://www.youtube.com/@noriyaro/videos").
     * @param _description YouTube Channel's description (e.g., "JP Drifting").
     */
    explicit Channel(const std::string &_name,
                     const std::string &_link,
                     const std::string &_description)
        : name(_name),
          link(_link),
          description(_description) {}

    /**
     * @brief Compare two YouTube channels for equality.
     *
     * @param other Other YouTube channel to compare (e.g., "{name: "Noriyaro", link: "https://www.youtube.com/@noriyaro/videos", description: "JP Drifting"}").
     *
     * @return True if the channels are equal, false otherwise.
     */
    bool operator==(const Channel &other) const
    {
        return this->name == other.name &&
               this->link == other.link &&
               this->description == other.description;
    }

    /**
     * @brief YouTube Channel's name (e.g., "Noriyaro").
     */
    std::string name;

    /**
     * @brief YouTube Channel's link (e.g., "https://www.youtube.com/@noriyaro/videos").
     */
    std::string link;

    /**
     * @brief YouTube Channel's description (e.g., "JP Drifting").
     */
    std::string description;
};

/**
 * @brief Load a vector of YouTube channels from an HTML file on disk.
 *
 * @param input_path Path to the HTML file (e.g., "~/data.html").
 * @param create_backup If true, create a backup of the original file before saving (default: true).
 *
 * @return Alphabetically sorted (by name) vector of YouTube channels (e.g., {name: "Noriyaro", link: "https://www.youtube.com/@noriyaro/videos", description: "JP Drifting"}).
 *
 * @throws core::html::FileNotFoundError If the file does not exist.
 * @throws core::html::HtmlParseError If failed to parse the HTML file.
 * @throws core::html::IOError If the file cannot be opened for reading or if any other I/O error occurs.
 *
 * @note You can catch all exceptions listed above with core::html::IOError.
 */
[[nodiscard]] std::vector<Channel> load(const std::string &input_path,
                                        const bool create_backup = true);

/**
 * @brief Save a vector of YouTube channels to an HTML file on disk.
 *
 * @param output_path Path to the HTML file (e.g., "~/data.html").
 * @param channels Vector of YouTube channels (e.g., "{name: "Noriyaro", link: "https://www.youtube.com/@noriyaro/videos", description: "JP Drifting"}").
 *
 * @throws core::html::IOError If failed to save to disk.
 */
void save(const std::string &output_path,
          const std::vector<Channel> &channels);

}  // namespace core::html
