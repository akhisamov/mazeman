add_subdirectory(Components)
add_subdirectory(Prefabs)
add_subdirectory(Systems)

target_sources(mazeman PRIVATE
        Game.h Game.cpp
        main.cpp

        CMakeLists.txt
        )
