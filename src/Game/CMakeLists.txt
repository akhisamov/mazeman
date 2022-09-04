add_subdirectory(Components)
add_subdirectory(Systems)

target_sources(pacman PRIVATE
    Game.hpp Game.cpp
    main.cpp

    CMakeLists.txt
)
