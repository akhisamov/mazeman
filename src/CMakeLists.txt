add_subdirectory(Graphics)
add_subdirectory(Resources)
add_subdirectory(Utils)

target_sources(pacman PRIVATE
    Game.hpp Game.cpp
    main.cpp

    CMakeLists.txt
)
