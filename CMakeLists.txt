# Set the minimum acceptable version of CMake.
# To find your current version, run `cmake --version`.
cmake_minimum_required (VERSION "3.18")

# Set the project's name.
project ("yt-table")

# Set C++17 standard.
set (CMAKE_CXX_STANDARD "17")
set (CMAKE_CXX_STANDARD_REQUIRED "ON") # throw error at configure time if C++17 isn't supported by the compiler, instead of a compilation error

# Enable warnings (unused variables and other mistakes).
add_compile_options (
    "-Wall"
    "-Wextra"
    "-Werror" # turns all warnings into errors, which prevents `make && ./output` from running
    "-Wpedantic"
)

# Set default build variant to `Release`, unless specified otherwise (e.g., via VSCode's GUI settings).
# This greatly reduces filesize (e.g., 0.5MB -> 39KB).
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type." FORCE)
    message (STATUS "${PROJECT_NAME}: Setting build type to default (${CMAKE_BUILD_TYPE}), because no build type was specified.")
    # Set the possible values of build type for cmake-gui.
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# Create variable: directory containing the `.cpp` files.
set (
    SOURCE_DIRECTORY # set variable name
    "src"
)

# Create variable: target `.cpp` files to compile.
set (
    SOURCE_FILES # set variable name
    "${SOURCE_DIRECTORY}/main.cpp"
    "${SOURCE_DIRECTORY}/lib_args.cpp"
    "${SOURCE_DIRECTORY}/lib_disk.cpp"
)

# Set an output executable, created using the ".cpp" files listed above.
add_executable (
    ${PROJECT_NAME} # executable name: same as project name, i.e., `yt-table`
    ${SOURCE_FILES} # target `.cpp` files to compile
)

# Print build type to console, just in case.
message (STATUS "${PROJECT_NAME}: Build type: ${CMAKE_BUILD_TYPE}.")
