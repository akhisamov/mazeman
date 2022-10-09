CPMAddPackage(
    name SDL2
    VERSION 2.24.0
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.24.0
    OPTIONS
        "SDL_STATIC FALSE"
)
if (SDL_ADDED)
    set_target_properties(SDL2 PROPERTIES FOLDER "ThirdParty")
    set_target_properties(SDL2main PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE SDL2::SDL2main SDL2::SDL2)
endif()

CPMAddPackage(
    name SDL2_image
    VERSION 2.6.2
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG release-2.6.2
    OPTIONS
        "SDL2IMAGE_SAMPLES FALSE"
)
if (SDL_image_ADDED)
    set_target_properties(SDL2_image PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE SDL2_image::SDL2_image)
endif()

CPMAddPackage(
    name glad
    VERSION 0.1.36
    GITHUB_REPOSITORY Dav1dde/glad
    OPTIONS
        "GLAD_API \"gl=3.3\""
        "GLAD_PROFILE core"
        "GLAD_SPEC gl"
        "GLAD_GENERATOR c"
)
if (glad_ADDED)
    set_target_properties(glad PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE glad)
endif()

CPMAddPackage("gh:g-truc/glm#0.9.9.8")
if(glm_ADDED)
    set_target_properties(glm PROPERTIES FOLDER "ThirdParty")
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

CPMAddPackage(
    NAME json
    VERSION 3.11.2
    GITHUB_REPOSITORY nlohmann/json
    OPTIONS
        "JSON_BuildTests OFF"
        "JSON_Install OFF"
)
if (json_ADDED)
    set_target_properties(nlohmann_json PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(pacman PRIVATE nlohmann_json::nlohmann_json)
endif()