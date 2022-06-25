add_subdirectory(ResourceManager)
add_subdirectory(Resources)

target_sources(pacman PRIVATE
        Sprite.hpp Sprite.cpp
        SpriteRenderer.hpp SpriteRenderer.cpp
        StringUtils.hpp
        main.cpp
        )