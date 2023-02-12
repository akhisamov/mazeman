add_subdirectory(Components)
add_subdirectory(Systems)

target_sources(mazeman PRIVATE
        EntityRegistry.h EntityRegistry.cpp
        SystemRegistry.h SystemRegistry.cpp

        CMakeLists.txt
        )