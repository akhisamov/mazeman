cmake_minimum_required(VERSION 3.14)
project(Pac-Man C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/CPM.cmake)

add_executable(pacman)
target_include_directories(pacman PRIVATE src "${CMAKE_BINARY_DIR}/include")

add_subdirectory(third-party)
add_subdirectory(shaders)
add_subdirectory(src)

get_target_property(PACMAN_FILES pacman SOURCES)
foreach(FILE ${PACMAN_FILES})
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
