add_subdirectory(Resources)

target_sources(pacman PRIVATE
        Game.hpp Game.cpp
        Sprite.hpp Sprite.cpp
        SpriteRenderer.hpp SpriteRenderer.cpp
        StringUtils.hpp
        Window.hpp Window.cpp
        main.cpp
        )