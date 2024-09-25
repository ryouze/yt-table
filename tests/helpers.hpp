/**
 * @file helpers.hpp
 *
 * @brief Helper functions for automated tests.
 */

#pragma once

#include <filesystem>  // for std::filesystem

namespace helpers {

/**
 * @brief Class that represents a temporary directory as a RAII object.
 *
 * On construction, the class creates a temporary directory on disk. When the object goes out of scope, the directory is removed recursively from disk.
 *
 * @note This class is marked as `final` to prevent inheritance.
 */
class TempDir final {
  public:
    /**
     * @brief Construct a new TempDir object.
     *
     * On construction, the temporary directory is removed recursively from disk, then created again to ensure it is empty.
     *
     * @param directory Path to the temporary directory (e.g., "~/data").
     */
    inline explicit TempDir(const std::filesystem::path &directory)
        : directory_(directory)
    {
        std::filesystem::remove_all(this->directory_);
        std::filesystem::create_directories(this->directory_);
    }

    /**
     * @brief Destroy the TempDir object.
     *
     * On destruction, the temporary directory is removed recursively from disk.
     */
    inline ~TempDir()
    {
        std::filesystem::remove_all(this->directory_);
    }

    /**
     * @brief Get the path to the temporary directory.
     *
     * @return Const reference to a path to the temporary directory provided in the constructor (e.g., "~/data").
     */
    [[nodiscard]] inline const std::filesystem::path &get() const
    {
        return this->directory_;
    }

  private:
    /**
     * @brief Path to the temporary directory provided in the constructor (e.g., "~/data").
     */
    const std::filesystem::path directory_;
};

}  // namespace helpers
