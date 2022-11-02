# ---------------------------
#             SDL
# ---------------------------

find_package(SDL2 2.24.0 REQUIRED)
target_link_libraries(mazeman PRIVATE SDL2::Core SDL2::Main)
target_include_directories(mazeman PRIVATE stb)

# ---------------------------
#            GLAD
# ---------------------------

CPMAddPackage(
    name glad
    VERSION 2.0.1
    GITHUB_REPOSITORY Dav1dde/glad
)
if (glad_ADDED)
    message("${glad_SOURCE_DIR}")
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
