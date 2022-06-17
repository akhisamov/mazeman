add_subdirectory(ResourceManager)

target_sources(pacman PRIVATE
        Shader.hpp Shader.cpp
        Texture2D.hpp Texture2D.cpp
        SpriteRenderer.hpp SpriteRenderer.cpp
        Resource.hpp
        main.cpp
        )