target_sources(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_subdirectory(SDL)
target_link_libraries(pacman PRIVATE SDL2::SDL2main SDL2::SDL2)

add_subdirectory(SDL_image)
target_link_libraries(pacman PRIVATE SDL2_image::SDL2_image)

add_subdirectory(glm)
target_link_libraries(pacman PRIVATE glm::glm)

add_subdirectory(json)
target_link_libraries(pacman PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(physfs)
target_link_libraries(pacman PRIVATE physfs)
target_include_directories(pacman PRIVATE physfs/src)