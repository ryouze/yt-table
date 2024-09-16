include(FetchContent)

function(fetch_and_link_external_dependencies target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "[ERROR] Target '${target}' does not exist. Cannot fetch and link dependencies.")
  endif()

  set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
  set(FETCHCONTENT_QUIET OFF)
  set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/deps")

  # SYSTEM is used to prevent applying compile flags to the dependencies
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.0.2
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )

  FetchContent_Declare(
    pathmaster
    GIT_REPOSITORY https://github.com/ryouze/pathmaster.git
    GIT_TAG        v1.0.0
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
    EXCLUDE_FROM_ALL
    SYSTEM
  )

  # Make dependencies available
  FetchContent_MakeAvailable(fmt pathmaster)

  # Link dependencies to the target
  target_link_libraries(${target} PUBLIC fmt::fmt pathmaster)
  message(STATUS "[INFO] Linked dependencies 'fmt' and 'pathmaster' to target '${target}'.")
endfunction()
