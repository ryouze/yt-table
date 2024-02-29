# yt-table

yt-table is a command-line tool for managing YouTube subscriptions.


## Motivation

As someone who prefers not to use a Google account, I maintain my YouTube "subscriptions" through a locally-stored HTML table.

Of course, manually editing HTML is cumbersome. To streamline this process, I developed yt-table, a command-line tool that automates the management of these subscriptions. It offers two functionalities: adding a channel (`--add`) and removing a channel (`--remove`). I run it once a month, it's fast, and it gets the job done.


## Features

- Written in modern C++ (C++17).
- Comprehensive documentation with doxygen-style comments.
- No third-party dependencies.
- Automatic backups.
- Responsive, mobile-friendly CSS.


## Tested Systems

This project has been tested on the following systems:

- MacOS 14.3 (Sonoma)


## Requirements

To build and run this project, you'll need:

- C++17 or higher
- CMake


## Build

Follow these steps to build the project:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ryouze/yt-table.git
   ```

2. **Generate the build system**:
   ```bash
   cd yt-table
   mkdir build && cd build
   cmake ..
   ```

3. **Compile the project**:
    ```bash
    make -j
    ```

After successful compilation, you can run the program using `./yt-table`.

The mode is set to `Release` by default. To build in `Debug` mode, use `cmake -DCMAKE_BUILD_TYPE=Debug ..`.


## Usage

The program will create an empty `table.html` file on the first run.

As mentioned earlier, there are two modes: `--add` and `--remove`.


### a) Add

To add a channel, use `--add <NAME;DESCRIPTION;LINK>`.

E.g., `./yt-table --add 'Noriyaro;JP Drifting;https://www.youtube.com/@noriyaro/videos'`

No sanity checks are performed, apart from stripping extra whitespace from each item.

Linking the `Videos` tab of a YouTube channel is recommended, but you can link playlists or even cooking blogs.


### b) Remove

To remove a channel, use `--remove <NAME>`.

E.g., `./yt-table --remove 'Noriyaro'`

This might be a bit slow if the HTML table is big (i.e., it'll take more than 1 millisecond).

> NOTE: An automatic backup will be created (`table.html.old`) before writing any changes.


## Contributing

All contributions are welcome.


## License

This project is released into the public domain and is licensed under the Unlicense.
