/**
 * @file io.hpp
 *
 * @brief Input/output functions.
 */

#pragma once

#include <filesystem>  // for std::filesystem
#include <string>      // for std::string
#include <vector>      // for std::vector

namespace core::io {

/**
 * @brief Setup UTF-8 input/output on Windows. Do nothing on other platforms.
 *
 * @throws std::runtime_error If failed to enable UTF-8 encoding on Windows.
 */
void setup_utf8_console();

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
    [[nodiscard]] bool operator==(const Channel &other) const
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
 * @throws std::runtime_error If the file does not exist or if any other error occurs.
 */
[[nodiscard]] std::vector<Channel> load(const std::filesystem::path &input_path,
                                        const bool create_backup = true);

/**
 * @brief Save a vector of YouTube channels to an HTML file on disk.
 *
 * @param output_path Path to the HTML file (e.g., "~/data.html").
 * @param channels Vector of YouTube channels (e.g., "{name: "Noriyaro", link: "https://www.youtube.com/@noriyaro/videos", description: "JP Drifting"}").
 *
 * @throws std::runtime_error If failed to save to disk.
 */
void save(const std::filesystem::path &output_path,
          const std::vector<Channel> &channels);

}  // namespace core::io
