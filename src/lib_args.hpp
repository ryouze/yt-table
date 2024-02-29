#pragma once

#include <string>  // for std::string

namespace lib_args {

/**
 * @brief Table modification mode: `UNKNOWN`, `ADD`, `REMOVE`.
 * @details Enum classes require C++11.
 */
enum class mode_t {
    UNKNOWN,
    ADD,
    REMOVE,
};

/**
 * @brief Parse command line arguments, with default values used as fallback.
 */
class ArgParser {
  public:
    /**
     * @brief Check if files exist, assign command line arguments to variables. This will exit the program when `--help` called, and throw on error.
     * @param argc Number of arguments (ARGument Count).
     * @param argv Array of `char` pointers containing all arguments (ARGument Vector).
     */
    ArgParser(const int argc, char **argv);

    /**
     * @brief If true, print detailed processing information to stdout. Otherwise, maintain minimal output.
     */
    bool verbose = false;

    /**
     * @brief Filepath to the pretty output HTML table containing YouTube subscriptions. Can be overwritten through a command line argument.
     */
    std::string output = "table.html";

    /**
     * @brief Mode: add or remove a channel from the YouTube subscriptions table.
     * Should be overwritten through a command line argument (throw if not overwritten).
     * @details `UNKNOWN` is the default value, which will throw.
     */
    mode_t mode = mode_t::UNKNOWN;

    /**
     * @brief String provided to mode, used as argument.
     * Should be overwritten through a command line argument if `mode` is set.
     * E.g,, for `add`, the channel's name, category and link should be provided.
     */
    std::string mode_arg_string;
};

}  // namespace lib_args
