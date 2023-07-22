# yt-table
YouTube subscriptions manager.


---


# Explanation

I don't have a Google account (I don't like Google), so I keep my list of "subscriptions" as a locally-stored HTML table.

However, having to manually edit HTML code is quite tedious, so I decided to develop a simple tool to automate it.

It has two modes: adding a channel (`--add`) and removing a channel (`--remove`).


---


# Features

* Simple & fast
* Written in modern C++
* Extensive documentation
* Human-readable error messages
* Automatic backups
* Responsive, mobile-friendly CSS


---


# Setup

### 1. Create the `/build` directory and invoke CMake.

```bash
mkdir build && cd build
cmake ..
```

### 2. Compile using all CPU threads.

> NOTE: C++17 is required. The `Release` mode is enabled by default.

```bash
make -j
```

That's it.

You can now run `./yt-table`.


---


# Usage

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
