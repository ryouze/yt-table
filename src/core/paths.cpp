/**
 * @file paths.cpp
 */

#include <array>         // for std::array
#include <cstdlib>       // for std::getenv
#include <filesystem>    // for std::filesystem::path
#include <stdexcept>     // for std::runtime_error
#include <string>        // for std::string
#include <system_error>  // for std::system_error
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <Shlobj.h>          // for SHGetFolderPathW
#include <windows.h>         // for HRESULT, SUCCEEDED
#else                        // Assume POSIX for macOS and GNU/Linux
#include <pwd.h>             // for getpwuid_r, struct passwd
#include <sys/types.h>       // for uid_t
#include <unistd.h>          // for getuid
#endif

#include <fmt/core.h>

#include "paths.hpp"

namespace core::paths {

#if !defined(_WIN32)
namespace {

/**
 * @brief Private helper function to get the home directory on POSIX systems.
 *
 * @return Path to the home directory (e.g., "/home/user").
 *
 * @throws std::runtime_error If the home directory cannot be retrieved.
 */
std::filesystem::path get_home_directory()
{
    const char *home = std::getenv("HOME");
    if (home && home[0] != '\0') {
        return std::filesystem::path(home);
    }

    // Fallback to passwd database
    struct passwd pwd;
    struct passwd *result = nullptr;
    std::array<char, 1024> buffer{};  // Zero-initialize the buffer
    const uid_t uid = getuid();

    if (getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &result) == 0 && result) {
        if (pwd.pw_dir && pwd.pw_dir[0] != '\0') {
            return std::filesystem::path(pwd.pw_dir);
        }
    }
    throw std::runtime_error("Failed to get home directory on POSIX system");
}

}  // namespace
#endif  // !defined(_WIN32)

std::filesystem::path get_resources_directory(const std::filesystem::path &application_name,
                                              const bool create_directories)
{
    std::filesystem::path resources_path;

    try {
#if defined(__APPLE__)
        // macOS: "~/Library/Application Support/<application_name>/Resources"
        const std::filesystem::path home_dir = get_home_directory();
        resources_path = home_dir / "Library" / "Application Support" / application_name / "Resources";
#elif defined(_WIN32)
        // Windows: "C:/Users/<username>/AppData/Local/<application_name>/Resources"
        wchar_t local_appdata_path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_appdata_path))) {
            resources_path = std::filesystem::path(local_appdata_path) / application_name / "Resources";
        }
        else {
            throw std::runtime_error("Failed to get LocalAppData directory on Windows");
        }
#else
        // GNU/Linux: Use XDG_DATA_HOME or default to "~/.local/share/<application_name>"
        const char *xdg_data_home = std::getenv("XDG_DATA_HOME");
        std::filesystem::path data_home;

        if (xdg_data_home && xdg_data_home[0] != '\0') {
            data_home = std::filesystem::path(xdg_data_home);
        }
        else {
            const std::filesystem::path home_dir = get_home_directory();
            data_home = home_dir / ".local" / "share";
        }

        resources_path = data_home / application_name;
#endif

        // Make the path absolute and normalized
        resources_path = std::filesystem::absolute(resources_path).lexically_normal();

        // Optionally create the directory
        if (create_directories) {
            if (!std::filesystem::exists(resources_path)) {
                std::filesystem::create_directories(resources_path);
            }
            else if (!std::filesystem::is_directory(resources_path)) {
                throw std::runtime_error(fmt::format("Path exists but is not a directory: {}", resources_path.string()));
            }
        }

        return resources_path;
    }
    catch (const std::exception &e) {
        // Re-throw any other exceptions as std::runtime_error
        throw std::runtime_error(fmt::format("Failed to get resources directory: {}", e.what()));
    }
}

}  // namespace core::paths
