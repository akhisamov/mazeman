add_subdirectory(Components)
add_subdirectory(Systems)

target_sources(mazeman PRIVATE
    EntityRegistry.hpp EntityRegistry.cpp
    SystemRegistry.hpp

    CMakeLists.txt
)