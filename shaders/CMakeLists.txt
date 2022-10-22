set(SHADERS
    sprite.glsl
)

set(OUT_PATH "${CMAKE_BINARY_DIR}/include/Generated/shaders")
foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    set(OUT_FILE "${OUT_PATH}/${SHADER_NAME}.hpp")

    set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}")

    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_PATH}"
        COMMAND lua "${CMAKE_SOURCE_DIR}/tools/embed.lua" ${FULL_PATH} ${OUT_FILE}
        DEPENDS ${FULL_PATH})

    list(APPEND OUT_FILES ${OUT_FILE})
    list(APPEND SHADER_FILES ${FULL_PATH})
endforeach(SHADER)

add_custom_target(
    Shaders
    DEPENDS ${OUT_FILES}
    SOURCES ${SHADER_FILES}
)

add_dependencies(pacman Shaders)