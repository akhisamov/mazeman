target_sources(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_subdirectory(SDL)
target_link_libraries(pacman PRIVATE SDL2::SDL2main SDL2::SDL2)

set(BUILD_SHARED_LIBS FALSE)
set(SDL2IMAGE_SAMPLES FALSE)
add_subdirectory(SDL_image)
target_link_libraries(pacman PRIVATE SDL2_image::SDL2_image-static)

add_subdirectory(glm)
target_link_libraries(pacman PRIVATE glm::glm)

option(PHYSFS_BUILD_SHARED "" FALSE)
option(PHYSFS_BUILD_TEST "" FALSE)
add_subdirectory(physfs)
target_link_libraries(pacman PRIVATE physfs-static)
target_include_directories(pacman PRIVATE physfs/src)
