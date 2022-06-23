add_subdirectory(ResourceManager)
add_subdirectory(Resources)

target_sources(pacman PRIVATE
        SpriteRenderer.hpp SpriteRenderer.cpp
        Sprite.hpp Sprite.cpp
        StringUtils.hpp
        main.cpp
        )