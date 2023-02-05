add_subdirectory(Shaders)

target_sources(mazeman PRIVATE
        AssetMaker.h
        AssetsManager.h AssetsManager.cpp
        IAsset.h
        Shader.h Shader.cpp
        ShaderMaker.h ShaderMaker.cpp
        Texture2D.h Texture2D.cpp
        Texture2DMaker.h Texture2DMaker.cpp
        World.h World.cpp
        WorldMaker.h WorldMaker.cpp
        )