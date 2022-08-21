target_sources(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

set(SDL_STATIC FALSE)
add_subdirectory(SDL)
set_target_properties(SDL2 PROPERTIES FOLDER "ThirdParty")
set_target_properties(SDL2main PROPERTIES FOLDER "ThirdParty")
target_link_libraries(pacman PRIVATE SDL2::SDL2main SDL2::SDL2)

if (NOT MSVC)
    set(BUILD_SHARED_LIBS FALSE)
endif()
set(SDL2IMAGE_SAMPLES FALSE)
add_subdirectory(SDL_image)

if (BUILD_SHARED_LIBS)
    set_target_properties(SDL2_image PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE SDL2_image::SDL2_image)
else()
    set_target_properties(SDL2_image-static PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE SDL2_image::SDL2_image-static)
endif()

CPMAddPackage("gh:g-truc/glm#0.9.9.8")
if(glm_ADDED)
    target_link_libraries(pacman PRIVATE glm::glm)
endif()

CPMAddPackage(
  NAME physfs
  VERSION 3.0.2
  GITHUB_REPOSITORY icculus/physfs
  GIT_TAG release-3.0.2
  OPTIONS
    "PHYSFS_BUILD_SHARED OFF"
    "PHYSFS_BUILD_TEST OFF"
)
if(physfs_ADDED)
    set_target_properties(physfs-static PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE physfs-static)
    target_include_directories(pacman PRIVATE ${physfs_SOURCE_DIR}/src)
endif()
