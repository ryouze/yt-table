#include "lib_args.hpp"  // lib_args::ArgParser
#include "lib_disk.hpp"  // lib_disk::HTMLFile
#include <filesystem>    // [C++17 required] std::filesystem::filesystem_error
#include <iostream>      // std::cout, std::cerr
#include <stdexcept>     // std::runtime_error

int main(int argc, char **argv)
{
    try {
        // process command line arguments
        const lib_args::ArgParser args{argc, argv};
        // add or remove channel
        lib_disk::HTMLFile file{args.output};
        switch (args.mode) {
        case lib_args::mode_t::ADD:
            file.add(args.mode_arg_string);
            std::cout << "Channel added to '" << args.output << "'.\n";
            break;
        case lib_args::mode_t::REMOVE:
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
    catch (const std::filesystem::filesystem_error &e) {  // derived from `std::runtime_error`
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown error.\n";
    }
    return 0;
}
