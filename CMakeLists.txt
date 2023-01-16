cmake_minimum_required(VERSION 3.14)
project(MazeMan CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/CPM.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

add_executable(mazeman)
target_include_directories(mazeman PRIVATE src "${CMAKE_BINARY_DIR}/include")

add_custom_target(Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)
add_dependencies(mazeman Resources)

add_subdirectory(third-party)
add_subdirectory(shaders)
add_subdirectory(src)

get_target_property(MAZEMAN_FILES mazeman SOURCES)
foreach(FILE ${MAZEMAN_FILES})
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
