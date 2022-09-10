add_subdirectory(Components)
add_subdirectory(Systems)

target_sources(pacman PRIVATE
    EntityRegistry.hpp EntityRegistry.cpp

    CMakeLists.txt
)