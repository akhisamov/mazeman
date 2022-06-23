add_subdirectory(ResourceManager)
add_subdirectory(Resources)

target_sources(pacman PRIVATE
        SpriteRenderer.hpp SpriteRenderer.cpp
        StringUtils.hpp
        main.cpp
        )