/**
 * @file disk.cpp
 */

#include <algorithm>  // for std::find_if
#include <string>     // for std::string
#include <vector>     // for std::vector

#include "core/html.hpp"
#include "disk.hpp"

modules::disk::Table::Table(const std::string &file_path)
    : filepath_(file_path)
{
    try {

        // Load the HTML table from disk
        // This will backup the file before loading, so we can safely overwrite if we want to
        this->channels_ = core::html::load(this->filepath_);
    }
    catch (const core::html::FileNotFoundError &) {
        // If file doesn't exist, write an empty table to disk
        core::html::save(this->filepath_, this->channels_);
    }
}

void modules::disk::Table::add(const core::html::Channel &channel)
{
    this->channels_.emplace_back(channel);
    // Save to disk
    this->save();
}

bool modules::disk::Table::remove(const std::string &name)
{
    // Find the channel by name
    const auto it = std::find_if(this->channels_.cbegin(), this->channels_.cend(),
                                 [&name](const core::html::Channel &channel) {
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

const std::string &modules::disk::Table::get_filepath() const
{
    return this->filepath_;
}

const std::vector<core::html::Channel> &modules::disk::Table::get_channels() const
{
    return this->channels_;
}

void modules::disk::Table::save()
{
    core::html::save(this->filepath_, this->channels_);
}
