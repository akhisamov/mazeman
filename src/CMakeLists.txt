add_subdirectory(Graphics)
add_subdirectory(Resources)

target_sources(pacman PRIVATE
        Game.hpp Game.cpp
        StringUtils.hpp

        main.cpp
        )