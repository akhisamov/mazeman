add_subdirectory(Components)
add_subdirectory(Prefabs)
add_subdirectory(Systems)

target_sources(pacman PRIVATE
    Game.hpp Game.cpp
    main.cpp

    CMakeLists.txt
)
