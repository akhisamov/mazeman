add_subdirectory(Components)
add_subdirectory(GUI)
add_subdirectory(Prefabs)
add_subdirectory(Systems)

target_sources(mazeman PRIVATE
    Game.hpp Game.cpp
    main.cpp

    CMakeLists.txt
)
