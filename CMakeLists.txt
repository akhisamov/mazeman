cmake_minimum_required(VERSION 3.13)
project(Pac-Man CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(pacman)
target_include_directories(pacman PRIVATE src)

add_subdirectory(lib/sdl)
target_link_libraries(pacman PRIVATE SDL2::SDL2-static SDL2::SDL2main)

add_subdirectory(src)
