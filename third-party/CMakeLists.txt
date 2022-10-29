# ---------------------------
#             SDL
# ---------------------------

set(SDL_VERSION 2.24.0)
set(SDL_IMAGE_VERSION 2.6.0)

set(ALL_SDL_FOUND FALSE)
find_package(SDL2 ${SDL_VERSION})
if (SDL2_FOUND)
    find_package(SDL2_image ${SDL_IMAGE_VERSION})
    if (SDL2_image_FOUND)
        set(ALL_SDL_FOUND TRUE)
        target_link_libraries(mazeman PRIVATE SDL2::SDL2 SDL2::Image)
    endif()
endif()

if (NOT ALL_SDL_FOUND)
    CPMAddPackage(
        name SDL2
        VERSION ${SDL_VERSION}
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-${SDL_VERSION}
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
        VERSION ${SDL_IMAGE_VERSION}
        GITHUB_REPOSITORY libsdl-org/SDL_image
        GIT_TAG release-${SDL_IMAGE_VERSION}
        OPTIONS
            "SDL2IMAGE_SAMPLES FALSE"
            "SDL2IMAGE_INSTALL FALSE"
            "BUILD_SHARED_LIBS OFF"
    )
    if (SDL_image_ADDED)
        set_target_properties(SDL2_image PROPERTIES FOLDER "ThirdParty")
        target_link_libraries(mazeman PRIVATE SDL2_image::SDL2_image-static)
    endif()
endif()

# ---------------------------
#            GLAD
# ---------------------------

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

# ---------------------------
#             GLM
# ---------------------------

CPMAddPackage(
    name glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 0.9.9.8
    OPTIONS
      "BUILD_SHARED_LIBS FALSE"
)
if(glm_ADDED)
    set_target_properties(glm PROPERTIES FOLDER "ThirdParty")
    target_link_libraries(mazeman PRIVATE glm::glm)
endif()

# ---------------------------
#           physfs
# ---------------------------

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

# ---------------------------
#            json
# ---------------------------

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
