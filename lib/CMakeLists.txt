target_sources(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(pacman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

find_package(SDL2 2.0.22 CONFIG REQUIRED)
target_link_libraries(pacman PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(pacman PRIVATE SDL2::SDL2_image)

find_package(glm CONFIG REQUIRED)
target_link_libraries(pacman PRIVATE glm::glm)

find_package(libzip 1.8.0 CONFIG REQUIRED)
target_link_libraries(pacman PRIVATE libzip::zip)

find_package(nlohmann_json 3.10.5 CONFIG REQUIRED)
target_link_libraries(pacman PRIVATE nlohmann_json::nlohmann_json)