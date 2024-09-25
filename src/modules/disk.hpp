/**
 * @file disk.hpp
 *
 * @brief Disk operations.
 */

#pragma once

#include <string>  // for std::string
#include <vector>  // for std::vector

#include "core/io.hpp"

namespace modules::disk {

/**
 * @brief Class that represents an HTML table.
 *
 * On construction, the class loads an HTML table from disk.
 *
 * @note This class is marked as `final` to prevent inheritance.
 */
class Table final {
  public:
    /**
     * @brief Construct a new Table object.
     *
     * The underlying parser will backup the file before loading. If the file doesn't exist, an empty table will be written to disk.
     *
     * @param filepath Path to the HTML table that contains YouTube subscriptions which shall be loaded (e.g., "~/data.html").
     */
    explicit Table(const std::filesystem::path &filepath);

    /**
     * @brief Add a YouTube to the table. The full channel object must be provided.
     *
     * After adding, the table is immediately saved to disk.
     *
     * @param channel Channel to add (e.g., "{name: "Noriyaro", link: "https://www.youtube.com/@noriyaro/videos", description: "JP Drifting"}").
     */
    void add(const core::io::Channel &channel);

    /**
     * @brief Remove a YouTube channel from the table by name.
     *
     * After removing, the table is immediately saved to disk.
     *
     * @param name Name of the YouTube channel to remove (e.g., "Noriyaro").
     *
     * @return True if succeeded, false if failed to find the channel.
     */
    [[nodiscard]] bool remove(const std::string &name);

    /**
     * @brief Get the file path.
     *
     * @return Path to the HTML table that contains YouTube subscriptions.
     */
    [[nodiscard]] const std::filesystem::path &get_filepath() const;

    /**
     * @brief Get the channels.
     *
     * @return Vector of YouTube channels.
     */
    [[nodiscard]] const std::vector<core::io::Channel> &get_channels() const;

  private:
    /**
     * @brief Path to the HTML table that contains YouTube subscriptions.
     */
    const std::filesystem::path filepath_;

    /**
     * @brief Vector of YouTube channels.
     */
    std::vector<core::io::Channel> channels_;

    /**
     * @brief Save the YouTube channels to an HTML file on disk.
     */
    void save();
};

}  // namespace modules::disk
