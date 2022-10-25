CPMAddPackage(
    name SDL2
    VERSION 2.24.0
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.24.0
    OPTIONS
      "SDL_SHARED OFF"
      "SDL_TEST OFF"
)
if (SDL_ADDED)
    set_target_properties(SDL2-static PROPERTIES FOLDER "ThirdParty")
    set_target_properties(SDL2main PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(mazeman PRIVATE SDL2::SDL2main SDL2::SDL2-static)
endif()

CPMAddPackage(
    name SDL2_image
    VERSION 2.6.2
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG release-2.6.2
    OPTIONS
        "SDL2IMAGE_SAMPLES FALSE"
        "SDL2IMAGE_INSTALL FALSE"
        "BUILD_SHARED_LIBS OFF"
)
if (SDL_image_ADDED)
    set_target_properties(SDL2_image PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(mazeman PRIVATE SDL2_image::SDL2_image-static)
endif()

CPMAddPackage(
    name glad
    VERSION 2.0.1
    GITHUB_REPOSITORY Dav1dde/glad
)
if (glad_ADDED)
    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad_gl_core_33 STATIC API gl:core=3.3)
    target_link_libraries(mazeman PRIVATE glad_gl_core_33)
endif()

CPMAddPackage(
    name glm
    VERSION 0.9.9.8
    GITHUB_REPOSITORY g-truc/glm
    OPTIONS
      "BUILD_SHARED_LIBS FALSE"
)
if(glm_ADDED)
    set_target_properties(glm PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(mazeman PRIVATE glm::glm)
endif()

CPMAddPackage(
  NAME physfs
  VERSION 3.0.2
  GITHUB_REPOSITORY icculus/physfs
  GIT_TAG release-3.0.2
  OPTIONS
    "PHYSFS_BUILD_SHARED OFF"
    "PHYSFS_BUILD_TEST OFF"
    "PHYSFS_TARGETNAME_UNINSTALL physfs_uninstall"
)
if(physfs_ADDED)
    set_target_properties(physfs-static PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(mazeman PRIVATE physfs-static)
    target_include_directories(mazeman PRIVATE ${physfs_SOURCE_DIR}/src)
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
    target_link_libraries(mazeman PRIVATE nlohmann_json::nlohmann_json)
endif()