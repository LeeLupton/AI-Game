cmake_minimum_required(VERSION 3.26)

project(Blockforge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

find_package(fmt CONFIG QUIET)
if(NOT fmt_FOUND)
    message(STATUS "fmt not found via package manager; fetching with FetchContent")
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.2
    )
    FetchContent_MakeAvailable(fmt)
endif()

find_package(spdlog CONFIG QUIET)
if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found via package manager; fetching with FetchContent")
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
    )
    FetchContent_MakeAvailable(spdlog)
endif()

find_package(SDL2 CONFIG QUIET)
if(NOT SDL2_FOUND)
    message(STATUS "SDL2 not found via package manager; fetching with FetchContent")
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3
    )
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL2)
endif()

add_executable(Blockforge
    src/Core/Error.cpp
    src/Core/Log.cpp
    src/main.cpp
)

target_include_directories(Blockforge PRIVATE include)

target_link_libraries(Blockforge PRIVATE
    fmt::fmt
    spdlog::spdlog
    SDL2::SDL2
)

if(TARGET SDL2::SDL2main)
    target_link_libraries(Blockforge PRIVATE SDL2::SDL2main)
endif()

if(MSVC)
    target_compile_options(Blockforge PRIVATE /W4 /permissive- /WX)
else()
    target_compile_options(Blockforge PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Enable unicode on Windows builds.
if(WIN32)
    target_compile_definitions(Blockforge PRIVATE UNICODE _UNICODE)
endif()

install(TARGETS Blockforge)
