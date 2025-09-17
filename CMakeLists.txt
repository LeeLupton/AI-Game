cmake_minimum_required(VERSION 3.26)

project(Blockforge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

add_executable(Blockforge
    src/main.cpp
)

target_include_directories(Blockforge PRIVATE include)

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
