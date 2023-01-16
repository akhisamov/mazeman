target_include_directories(mazeman PRIVATE stb)

# ---------------------------
#             SDL
# ---------------------------

CPMAddPackage(
    NAME SDL2
    VERSION 2.26.0
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.26.0
    OPTIONS "SDL2_DISABLE_INSTALL TRUE"
)
if (SDL2_ADDED)
    target_include_directories(mazeman PRIVATE ${SDL2_SOURCE_DIR}/include)
    target_link_libraries(mazeman PRIVATE SDL2)
endif()

# ---------------------------
#            GLAD
# ---------------------------

add_subdirectory(glad)
target_link_libraries(mazeman PRIVATE glad)

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
  VERSION 3.2.0
  GITHUB_REPOSITORY icculus/physfs
  GIT_TAG release-3.2.0
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
