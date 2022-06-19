cmake_minimum_required(VERSION 3.22)
project(Pac-Man C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(pacman)
target_include_directories(pacman PRIVATE src)

add_subdirectory(lib)

add_subdirectory(src)
