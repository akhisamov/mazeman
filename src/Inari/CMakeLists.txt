add_subdirectory(ECS)
add_subdirectory(Graphics)
add_subdirectory(GUI)
add_subdirectory(Input)
add_subdirectory(Resources)
add_subdirectory(Utils)

target_sources(mazeman PRIVATE
    IGame.hpp IGame.cpp

    CMakeLists.txt
)
