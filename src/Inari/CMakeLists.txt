add_subdirectory(ECS)
add_subdirectory(Graphics)
add_subdirectory(Resources)
add_subdirectory(Utils)

target_sources(pacman PRIVATE
    IGame.hpp IGame.cpp

    CMakeLists.txt
)
