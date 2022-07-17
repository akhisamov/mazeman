add_subdirectory(Graphics)
add_subdirectory(Resources)

target_sources(pacman PRIVATE
        Game.hpp Game.cpp
        Camera2D.hpp Camera2D.cpp
        StringUtils.hpp

        main.cpp
        )
