# yt-table

[![CI](https://github.com/ryouze/yt-table/actions/workflows/ci.yml/badge.svg)](https://github.com/ryouze/yt-table/actions/workflows/ci.yml)
[![Release](https://github.com/ryouze/yt-table/actions/workflows/release.yml/badge.svg)](https://github.com/ryouze/yt-table/actions/workflows/release.yml)
![Release version](https://img.shields.io/github/v/release/ryouze/yt-table)

yt-table is a cross-platform CLI tool for managing YouTube subscriptions locally through a shell-like interface.


## Motivation

As someone who prefers not to use a Google account, I maintain my YouTube "subscriptions" through a locally-stored HTML table. Not only does it look better than regular bookmarks, but it also allows me to add descriptions for each channel.

However, manually editing HTML code is quite annoying and error-prone. To simplify this process, I developed yt-table, a command-line tool that generates a HTML table automatically. It offers a simple, shell-like interface for adding and removing channels.

It works like this:

```sh
[yt-table] $ add
Enter name: noriyaro
Enter description: Cars
Enter link: https://www.youtube.com/@noriyaro/videos
Channel 'noriyaro' added
```

```sh
[yt-table] $ ls

Channels (2):
  Name: noriyaro
  Link: https://www.youtube.com/@noriyaro/videos
  Description: Cars

  Name: Hugh Jeffreys
  Link: https://www.youtube.com/@HughJeffreys/videos
  Description: Phone Repairs
```

```sh
[yt-table] $ remove
Enter name: Hugh Jeffreys
Channel 'Hugh Jeffreys' removed
```

Under the hood, the tool uses regex to parse the HTML file and extract an array of channels. When a change is made, the tool rewrites the entire file, converting the modified array of channels to HTML code. The HTML table itself is stored in a platform-specific directory (e.g., `~/Library/Application Support/yt-table/Resources/subscriptions.html` on macOS), which can be opened (and bookmarked) in a web browser for easy access.


## Features

- Written in modern C++ (C++17).
- Comprehensive documentation with doxygen-style comments.
- Automatic backups.
- Responsive, mobile-friendly CSS.
- Automatic third-party dependency management using CMake's [FetchContent](https://www.foonathan.net/2022/06/cmake-fetchcontent/).
- No missing STL headers thanks to [header-warden](https://github.com/ryouze/header-warden).


## Tested Systems

This project has been tested on the following systems:

- macOS 14.6 (Sonoma)
- Manjaro 24.0 (Wynsdey)
- Windows 11 23H2

Automated testing is also performed on the latest versions of macOS, GNU/Linux, and Windows using GitHub Actions.


## Pre-built Binaries

Pre-built binaries are available for macOS (ARM64), GNU/Linux (x86_64), and Windows (x86_64). You can download the latest version from the [Releases](../../releases) page.

To remove macOS quarantine, use the following commands:

```sh
xattr -d com.apple.quarantine yt-table-macos-arm64
chmod +x yt-table-macos-arm64
```

On Windows, the OS might complain about the binary being unsigned. You can bypass this by clicking on "More info" and then "Run anyway".


## Requirements

To build and run this project, you'll need:

- C++17 or higher
- CMake


## Build

Follow these steps to build the project:

1. **Clone the repository**:

    ```sh
    git clone https://github.com/ryouze/yt-table.git
    ```

2. **Generate the build system**:

    ```sh
    cd yt-table
    mkdir build && cd build
    cmake ..
    ```

    Optionally, you can disable compile warnings by setting `ENABLE_COMPILE_FLAGS` to `OFF`:

    ```sh
    cmake .. -DENABLE_COMPILE_FLAGS=OFF
    ```

3. **Compile the project**:

    To compile the project, use the following command:

    ```sh
    cmake --build . --parallel
    ```

After successful compilation, you can run the program using `./yt-table`. However, it is highly recommended to install the program, so that it can be run from any directory. Refer to the [Install](#install) section below.

**Note:** The mode is set to `Release` by default. To build in `Debug` mode, use `cmake .. -DCMAKE_BUILD_TYPE=Debug`.


## Install

If not already built, follow the steps in the [Build](#build) section and ensure that you are in the `build` directory.

To install the program, use the following command:

```sh
sudo cmake --install .
```

On macOS, this will install the program to `/usr/local/bin`. You can then run the program from any directory using `yt-table`.


## Usage

To run the program, use the following command:

```sh
yt-table
```

**Note:** On Windows, a modern terminal emulator like [Windows Terminal](https://github.com/microsoft/terminal) is recommended. The default Command Prompt will display UTF-8 characters correctly, but UTF-8 input is not supported.

On startup, the program will create an empty `subscriptions.html` file in a platform-specific directory. Then, a shell-like interface will appear, allowing you to interact with the file.

The following commands are available:

- `help`: Print the help message.
- `version`: Print the version.
- `ls`: Print the list of channels.
- `open`: Open the HTML table in a web browser.
- `add`: Add a new channel (name, description, link).
- `remove`: Remove a channel (name).
- `exit`: Exit the program.

The changes are saved automatically and a backup file is created in the same directory as the `subscriptions.html` file. Any leading or trailing whitespace in the input is removed.

The program does not support history using the up/down arrow keys or other full terminal features. It is designed to be as simple as possible, because I primarily interact with the HTML table itself.


## Flags

```sh
[~] $ yt-table --help
Usage: yt-table [-h] [-v]

Manage YouTube subscriptions locally through a shell-like interface.

Optional arguments:
  -h, --help     prints help message and exits
  -v, --version  prints version and exits
```


## Testing

Tests are included in the project but are not built by default.

To enable and build the tests manually, run the following commands from the `build` directory:

```sh
cmake .. -DBUILD_TESTS=ON
cmake --build . --parallel
ctest --output-on-failure
```


## Credits

- [fmt](https://github.com/fmtlib/fmt)


## Contributing

All contributions are welcome.


## License

This project is licensed under the MIT License.
