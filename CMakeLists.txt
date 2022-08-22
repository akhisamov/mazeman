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

file(GLOB_RECURSE GLSL_VERT_FILES
    "shaders/*.vert"
)

file(GLOB_RECURSE GLSL_FRAG_FILES
    "shaders/*.frag"
)

foreach(VERT ${GLSL_VERT_FILES})
    get_filename_component(FILE_NAME ${VERT} NAME_WE)
    get_filename_component(FRAG_FILE ${VERT} DIRECTORY)
    string(APPEND FRAG_FILE "/${FILE_NAME}.frag")
    list(FIND GLSL_FRAG_FILES ${FRAG_FILE} FRAG_INDEX)
    list(GET GLSL_FRAG_FILES ${FRAG_INDEX} FRAG)
    set(GLSL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/Generated/shaders")
    set(GLSL_HEADER "${GLSL_PATH}/${FILE_NAME}.hpp")
    add_custom_command(
        OUTPUT ${GLSL_HEADER}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/Generated/shaders"
        COMMAND lua "${CMAKE_CURRENT_SOURCE_DIR}/tools/embedshader.lua" ${GLSL_HEADER} -- ${VERT} ${FRAG}
        DEPENDS ${VERT} ${FRAG})
    list(APPEND GLSL_HEADERS ${GLSL_HEADER})
endforeach(VERT)

add_custom_target(
    Shaders
    DEPENDS ${GLSL_HEADERS}
    SOURCES ${GLSL_VERT_FILES} ${GLSL_FRAG_FILES}
)
source_group("Vertex Shaders" FILES ${GLSL_VERT_FILES})
source_group("Fragment Shaders" FILES ${GLSL_FRAG_FILES})

add_executable(pacman)
target_include_directories(pacman PRIVATE src)

add_dependencies(pacman Shaders)

add_subdirectory(lib)

add_subdirectory(src)

get_target_property(PACMAN_FILES pacman SOURCES)
foreach(FILE ${PACMAN_FILES})
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
