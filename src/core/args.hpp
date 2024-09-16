/**
 * @file args.hpp
 *
 * @brief Process command-line arguments.
 */

#pragma once

#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace core::args {

/**
 * @brief Exceptions raised by command-line argument parser when help or version is requested. The requested message is returned.
 */
class ArgsMessage : public std::runtime_error {
  public:
    explicit ArgsMessage(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Exceptions raised by command-line argument parser when an error occurs. A help message with usage, description, and examples is included.
 */
class ArgsError : public std::runtime_error {
  public:
    explicit ArgsError(const std::string &message)
        : std::runtime_error(message) {}
};

/**
 * @brief Class that represents command-line arguments.
 *
 * On construction, the class parses the command-line arguments. If no arguments are provided, the class does nothing. If help or version is requested, the class throws an exception. Similarly, if an error occurs, the class also throws an exception.
 *
 * @note This class is marked as `final` to prevent inheritance.
 */
class Args final {
  public:
    /**
     * @brief Construct a new Args object.
     *
     * @param argc Number of command-line arguments (e.g., "2").
     * @param argv Array of command-line arguments (e.g., {"./bin", "-h"}).
     *
     * @throws ArgsMessage If a help or version message is requested.
     * @throws ArgsError If failed to process command-line arguments.
     */
    explicit Args(const int argc,
                  char **argv);
};

}  // namespace core::args
