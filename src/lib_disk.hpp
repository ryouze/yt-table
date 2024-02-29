#pragma once

#include <map>     // for std::map
#include <string>  // for std::string

namespace lib_disk {

/**
 * @brief Abstract class (=interface) for file management on disk. This class cannot be instantiated.
 */
class AbstractFile {
  public:
    /**
     * @brief Initialize a protected reference to a private string containing the filepath.
     * @param filepath Path to the file.
     */
    explicit AbstractFile(const std::string &filepath) : filepath_(filepath) {}

    /**
     * @brief Virtual destructor. Enables proper cleanup for derivded classes.
     */
    virtual ~AbstractFile() = default;

    /**
     * @brief Get status of the file as a formatted string with trailing newline.
     * @details This is a pure virtual function declaration that must be overriden by the child class.
     * @return Long, formatted string containing the status of the file.
     */
    [[nodiscard]] virtual std::string get_status() const = 0;

  protected:
    /**
     * @brief Reference to path to the file.
     */
    const std::string &filepath_;
};

/**
 * @brief File management abstraction for the HTML table file on disk. This class can be instantiated.
 */
class HTMLFile : public AbstractFile {
    // public, because we want base Abstract class's methods (`get_status()`) to be accessible from the outside
  public:
    /**
     * @brief Read HTML file from disk (create placeholder if doesn't exist), extract current subscriptions list.
     * @details A protected reference to filepath is also initalized.
     * @param filepath Path to the file.
     */
    explicit HTMLFile(const std::string &filepath);

    /**
     * @brief Get status of the HTML file as a formatted string.
     * @return Long, formatted string containing the status of the HTML file.
     */
    [[nodiscard]] std::string get_status() const override;

    /**
     * @brief Add channel to subscriptions list. Use `this->write_to_disk()` to commit changes. If failed, throw.
     * @param string_to_split Name of the channel, very short description of the channel, link to the channel, e.g., `Noriyaro;JP Drifting;https://www.youtube.com/@noriyaro/videos/`. This is case sensitive.
     */
    void add(const std::string &string_to_split);

    /**
     * @brief Remove channel from subscriptions list. Use `this->write_to_disk()` to commit changes. If failed, throw.
     * @details This function is quite slow, because all keys have to be checked. This program was designed around adding channels instead of removing them, so that's where performance lies.
     * @param name Name of the channel, e.g., `Noriyaro`. This is case sensitive.
     */
    void remove(const std::string &name);

    /**
     * @brief Save edited HTML table to disk. If failed, throw.
     */
    void write_to_disk() const;

  private:
    /**
     * @brief Force rename file on disk. The file at `new_filepath` will be deleted if it already exists. If failed, throw.
     * @details `std::filesystem` requires C++17.
     * @param new_filepath The new name of the file. This may include a directory, e.g., `Pictures/Car.png`.
     */
    inline void force_rename(const std::string &new_filepath) const;

    /**
     * @brief Remove leading and trailing whitespace from string. Note: this returns a copy.
     * @details Example usage: `const std::string s_trimmed = this->remove_whitespace(s);`
     * @param str String to remove whitespace from.
     * @param whitespace String containing characters to remove (default=`" \t"`).
     * @return String without whitespace.
     */
    [[nodiscard]] static std::string remove_whitespace(const std::string &str, const std::string &whitespace = " \t");

    /**
     * @brief Map containing YouTube subscriptions. Key = `<td>` tag containing link. Value = short description of the channel.
     */
    std::map<std::string, std::string> subscriptions_;
};

}  // namespace lib_disk
