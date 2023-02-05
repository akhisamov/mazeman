add_subdirectory(Assets)
add_subdirectory(ECS)
add_subdirectory(Graphics)
add_subdirectory(Utils)

target_sources(mazeman PRIVATE
        GameServices.h GameServices.cpp
        IGame.h IGame.cpp
        InputManager.hpp InputManager.cpp
        IService.h

        CMakeLists.txt
        )
