#include "lib_args.hpp"

#include <algorithm>  // for std::find
#include <cstdlib>    // for std::exit, EXIT_SUCCESS
#include <iostream>   // for std::cout
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string
#include <vector>     // for std::vector

/**
 * @brief Helper utilities, only accessible within this CPP file.
 */
namespace {

/**
 * @brief A hidden abstraction that facilitates the processing of command line arguments, such as treating two arguments as a keyword pair.
 * @details It is used by `ArgParser` and cannot be accessed from the outside of this CPP file. This prevents unecessary clutter in the header.
 */
class ArgParsingHelperUtil {
  public:
    /**
     * @brief Initialize a private reference to a private string vector containing the command line arguments.
     * @details The `explicit` keyword prevents implicit conversions (e.g., `double 2.0` to `int 2`).
     * Always use it for single-parameter constructors, especially primitive types such as `int`, `float` and `char`.
     * For `std::string`, the default (=implicit) is okay, because it will convert `char*` to `std::string`, which is the the expected behavior in most cases.
     * @param arguments String vector, where each item is an argument.
     */
    explicit ArgParsingHelperUtil(const std::vector<std::string> &arguments) : arguments_(arguments) {}

    /**
     * @brief Check if a single argument exists, e.g., `./main.o -h`. The leading dashes must be included.
     * @param arg_name Argument to look for, e.g., `-h`.
     * @return True if exists, False if doesn't exist.
     */
    [[nodiscard]] inline bool check_if_exists(const std::string &arg_name) const
    {
        return (std::find(this->arguments_.begin(), this->arguments_.end(), arg_name) != this->arguments_.end());
    }

    /**
     * @brief Check if two arguments exist, e.g., `./main.o -h || ./main.o --help`. The leading dashes must be included.
     * @param arg_name1 First argument to look for, e.g., `-h`.
     * @param arg_name2 Second argument to look for, e.g., `--help`.
     * @return True if at least one of the arguments exists, False if none exist.
     */
    [[nodiscard]] inline bool check_if_exists(const std::string &arg_name1,
                                              const std::string &arg_name2) const
    {
        return (this->check_if_exists(arg_name1) || this->check_if_exists(arg_name2));
    }

    /**
     * @brief Get value of a keyword argument by key, e.g., `./main.o --filepath "shakespeare.txt"`. The leading dashes must be included.
     * @details I.e., if `arg_name` is `--filepath`, then the function would return `shakespeare.txt`.
     * @param arg_name Argument to look for, e.g., `--filepath`.
     * @return Value if exists, raise if doesn't exist.
     */
    [[nodiscard]] std::string get_keyword_pair(const std::string &arg_name) const;

  private:
    /**
     * @brief A reference to a string vector, where each item is an argument (as extracted from `argv`).
     */
    const std::vector<std::string> &arguments_;
};

std::string ArgParsingHelperUtil::get_keyword_pair(const std::string &arg_name) const
{
    // get iterator to the first element is equal to name.
    std::vector<std::string>::const_iterator itr = std::find(this->arguments_.begin(), this->arguments_.end(), arg_name);
    // get element AFTER that iterator (+1)
    if (itr != this->arguments_.end() && ++itr != this->arguments_.end()) {
        return *itr;
    }
    throw std::runtime_error("The keyword argument '" + arg_name + "' does not contain a value (e.g., `--remove 'CToretto'`).");
}

}  // namespace

lib_args::ArgParser::ArgParser(const int argc, char **argv)
{
    /**
     * @brief C-style arguments (`char **argv`) converted to a a string vector (`std::string`).
     */
    const std::vector<std::string> arguments(argv, argv + argc);
    /**
     * @brief An instance of the high-level argument parser; only available within the scope of `ArgParser`'s constructor.
     */
    const ArgParsingHelperUtil helper{arguments};
    // OPTIONAL: print help and exit program
    if (helper.check_if_exists("-h", "--help")) {
        std::cout
            << "usage: " << arguments.at(0) << " [-h | --help] [-v | --verbose] [--output <PATH>] [--add <NAME;DESCRIPTION;LINK> | --remove <NAME>]\n\n"
            << "options:\n"
            << "  -h | --help                    print this help message\n"
            << "  -v | --verbose                 display detailed processing information\n"
            << "  --output <PATH>                path to output HTML table (" << this->output << ")\n"
            << "  --add <NAME;DESCRIPTION;LINK>  add channel to HTML table\n"
            << "  --remove <NAME>                remove channel from HTML table\n\n"
            << "example 1: " << arguments.at(0) << " --add 'Noriyaro;JP Drifting;https://www.youtube.com/@noriyaro/videos'\n"
            << "example 2: " << arguments.at(0) << " --remove 'Noriyaro'\n";
        std::exit(EXIT_SUCCESS);
    }
    else {  // else statement for maximum clarity
        // OPTIONAL: verbose flag
        if (helper.check_if_exists("-v", "--verbose")) {
            this->verbose = true;
        }
        // OPTIONAL: custom filepath to subscriptions table
        if (helper.check_if_exists("--output")) {
            this->output = helper.get_keyword_pair("--output");
        }
        // -- mandatory arguments begin below this line --
        // MANDATORY/ELSE: add channel to subscriptions table
        if (helper.check_if_exists("--add")) {
            this->mode = mode_t::ADD;
            this->mode_arg_string = helper.get_keyword_pair("--add");  // set string, e.g., `--add foo`
        }
        // MANDATORY/ELSE: remove channel from subscriptions table
        else if (helper.check_if_exists("--remove")) {
            this->mode = mode_t::REMOVE;
            this->mode_arg_string = helper.get_keyword_pair("--remove");  // set string, e.g., `--remove foo`
        }
        // NO ARGUMENT: do nothing, this will be caught later
    }
}
