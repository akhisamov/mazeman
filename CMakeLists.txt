cmake_minimum_required(VERSION 3.13)
project(Pac-Man C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE GLSL_VERT_FILES
    "assets/shaders/*.vert"
)

file(GLOB_RECURSE GLSL_FRAG_FILES
    "assets/shaders/*.frag"
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
        DEPENDS ${VERT})
    list(APPEND GLSL_HEADERS ${GLSL_HEADER})
endforeach(VERT)

add_custom_target(
    Shaders
    DEPENDS ${GLSL_HEADERS}
)

add_executable(pacman)
target_include_directories(pacman PRIVATE src)

add_dependencies(pacman Shaders)

add_subdirectory(lib)

add_subdirectory(src)
