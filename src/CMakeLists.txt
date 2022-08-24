add_subdirectory(Graphics)
add_subdirectory(Resources)
add_subdirectory(Utils)

target_sources(pacman PRIVATE
    Game.hpp Game.cpp
    Camera2D.hpp Camera2D.cpp
    SpriteBatch.hpp SpriteBatch.cpp
    Transformable.hpp Transformable.cpp
    main.cpp

    CMakeLists.txt
)
