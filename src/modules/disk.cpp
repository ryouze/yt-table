/**
 * @file disk.cpp
 */

#include <algorithm>   // for std::find_if
#include <filesystem>  // for std::filesystem
#include <stdexcept>   // for std::runtime_error
#include <string>      // for std::string
#include <vector>      // for std::vector

#include "core/io.hpp"
#include "disk.hpp"

namespace modules::disk {

Table::Table(const std::filesystem::path &filepath)
    : filepath_(filepath)
{
    try {

        // Load the HTML table from disk
        // This will backup the file before loading, so we can safely overwrite if we want to
        this->channels_ = core::io::load(this->filepath_);
    }
    catch (const std::runtime_error &) {
        // If file doesn't exist or other error occurred, write an empty table to disk
        core::io::save(this->filepath_, this->channels_);
    }
}

void Table::add(const core::io::Channel &channel)
{
    this->channels_.emplace_back(channel);
    // Save to disk
    this->save();
}

bool Table::remove(const std::string &name)
{
    // Find the channel by name
    const auto it = std::find_if(this->channels_.cbegin(), this->channels_.cend(),
                                 [&name](const core::io::Channel &channel) {
                                     return channel.name == name;
                                 });

    // If the channel is found, remove it, and save to disk
    if (it != this->channels_.cend()) {
        this->channels_.erase(it);
        this->save();
        return true;
    }

    return false;
}

const std::filesystem::path &Table::get_filepath() const
{
    return this->filepath_;
}

const std::vector<core::io::Channel> &Table::get_channels() const
{
    return this->channels_;
}

void Table::save()
{
    // Write current state to disk
    core::io::save(this->filepath_, this->channels_);
}

}  // namespace modules::disk
