set(SHADERS
    "sprite.hpp sprite.frag sprite.vert"
)
set(OUT_PATH "${CMAKE_BINARY_DIR}/include/Generated/shaders")
foreach(SHADER_STR_LIST ${SHADERS})

    # transform string to list
    string(REPLACE " " ";" SHADER_LIST ${SHADER_STR_LIST})

    # pop out file
    list(GET SHADER_LIST 0 OUT_FILE)
    set(OUT_FILE "${OUT_PATH}/${OUT_FILE}")
    list(REMOVE_AT SHADER_LIST 0)

    # set full path to shader files
    foreach(SHADER ${SHADER_LIST})
        list(APPEND NEW_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}")
    endforeach(SHADER)
    set(SHADER_LIST ${NEW_LIST})

    # command
    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_PATH}"
        COMMAND lua "${CMAKE_SOURCE_DIR}/tools/embedshader.lua" ${OUT_FILE} -- ${SHADER_LIST}
        DEPENDS ${SHADER_LIST})

    # store all files
    list(APPEND OUT_FILES ${OUT_FILE})
    list(APPEND SHADER_FILES ${SHADER_LIST})

endforeach(SHADER_STR_LIST)

add_custom_target(
    Shaders
    DEPENDS ${OUT_FILE}
    SOURCES ${SHADER_FILES}
)

add_dependencies(pacman Shaders)