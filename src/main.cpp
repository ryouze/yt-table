#include "lib_args.hpp"
#include "lib_disk.hpp"

#include <cstdlib>    // for std::exit, EXIT_FAILURE
#include <ios>        // for std::ios_base
#include <iostream>   // for std::cout, std::cerr
#include <stdexcept>  // for std::runtime_error

int main(int argc, char **argv)
{
    try {
        // Disable synchronization between the C++ standard streams (e.g., std::cin, std::cout) and their corresponding C equivalents
        std::ios_base::sync_with_stdio(false);
        // process command line arguments
        const lib_args::ArgParser args{argc, argv};
        // create the HTML placeholder if doesn't exist; add or remove channel
        lib_disk::HTMLFile file{args.output};
        switch (args.mode) {
        case lib_args::ArgMode::ADD:
            file.add(args.mode_arg_string);
            std::cout << "Channel added to '" << args.output << "'.\n";
            break;
        case lib_args::ArgMode::REMOVE:
            file.remove(args.mode_arg_string);
            std::cout << "Channel removed from '" << args.output << "'.\n";
            break;
        default:
            throw std::runtime_error("No `--add` or `--remove` mode argument was provided. Use `--help` to display examples.");
        }
        // if verbose mode is enabled (via `--verbose` flag), print all table rows
        if (args.verbose) {
            std::cout << file.get_status();  // trailing newline is included
        }
        // save to disk
        file.write_to_disk();
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
    catch (...) {
        std::cerr << "Unknown error.\n";
        std::exit(EXIT_FAILURE);
    }
    return 0;
}
