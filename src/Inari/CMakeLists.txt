add_subdirectory(ECS)
add_subdirectory(Graphics)
add_subdirectory(Resources)
add_subdirectory(Utils)

target_sources(mazeman PRIVATE
    IGame.hpp IGame.cpp
    InputManager.hpp InputManager.cpp

    CMakeLists.txt
)
