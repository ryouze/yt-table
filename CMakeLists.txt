# Specify the minimum version of CMake that can be used.
# To check your current version, use the command `cmake --version`.
cmake_minimum_required (VERSION "3.18")

# Define the project name.
project ("yt-table")

# Enforce the use of C++17 standard.
# If the compiler does not support C++17, an error will be thrown during configuration, not during compilation.
set (CMAKE_CXX_STANDARD "17")
set (CMAKE_CXX_STANDARD_REQUIRED "ON")

# Enable compiler warnings for potential code issues (like unused variables).
# The "-Werror" flag treats all warnings as errors, halting the build process if any warning is encountered.
add_compile_options (
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Wpedantic"
)

# Set the default build type to `Release` if not specified otherwise (e.g., via VSCode's GUI settings).
# This can significantly reduce the size of the output file (e.g., from 0.5MB to 39KB).
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type." FORCE)
    message (STATUS "${PROJECT_NAME}: Defaulting build type to '${CMAKE_BUILD_TYPE}' as no specific type was provided.")
    # Define the possible values of build type for cmake-gui.
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# Define the directory that contains the source `.cpp` files.
set (
    SOURCE_DIRECTORY
    "src"
)

# Define the source `.cpp` files to be compiled.
set (
    SOURCE_FILES
    "${SOURCE_DIRECTORY}/main.cpp"
    "${SOURCE_DIRECTORY}/lib_args.cpp"
    "${SOURCE_DIRECTORY}/lib_disk.cpp"
)

# Define the output executable, which is created by compiling the source files defined above.
add_executable (
    ${PROJECT_NAME} # The name of the executable will be the same as the project name, i.e., `yt-table`.
    ${SOURCE_FILES} # The source files to be compiled.
)

# Output the build type to the console for verification.
message (STATUS "${PROJECT_NAME}: Build type: ${CMAKE_BUILD_TYPE}.")
