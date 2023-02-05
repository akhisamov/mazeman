#include "AssetsManager.h"

#include <SDL_filesystem.h>

#include <physfs.h>

#include "Inari/Assets/ShaderMaker.h"
#include "Inari/Assets/Texture2DMaker.h"
#include "Inari/Assets/WorldMaker.h"

#include <iostream>

namespace inari {
    std::shared_ptr<AssetsManager> AssetsManager::create()
    {
        if (PHYSFS_isInit()) {
            return nullptr;
        }

        if (PHYSFS_init(nullptr) == 0) {
            std::cerr << "AssetsManager Init Error: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << "\n";
            return nullptr;
        }

        return std::make_shared<AssetsManager>(AssetsManager::Token {});
    }

    AssetsManager::AssetsManager(AssetsManager::Token /*unused*/) { }
    AssetsManager::~AssetsManager() = default;

    void AssetsManager::addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint)
    {
        assert(PHYSFS_isInit());

        std::string absolutePath = SDL_GetBasePath();
        absolutePath += searchPath.data();
        PHYSFS_mount(absolutePath.c_str(), mountPoint.data(), 1);
    }

    void AssetsManager::addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths)
    {
        for (const auto& it : searchPaths) {
            addSearchPath(it.first, it.second);
        }
    }

    bool AssetsManager::unload(const std::string_view& name)
    {
        const AssetID id = std::hash<std::string_view> {}(name);
        return m_assets.erase(id) != 0;
    }

    bool AssetsManager::has(const std::string_view& name)
    {
        const AssetID id = std::hash<std::string_view> {}(name);
        return m_assets.find(id) != m_assets.end();
    }

    std::string AssetsManager::readFileData(const std::string_view& filename)
    {
        const char* filenameCStr = filename.data();
        const auto& it = m_filesDataByName.find(filenameCStr);
        if (it != m_filesDataByName.end()) {
            return it->second;
        }

        assert(PHYSFS_isInit());
        assert(PHYSFS_exists(filenameCStr) != 0);

        PHYSFS_File* file = PHYSFS_openRead(filenameCStr);
        assert(file);

        std::string buffer;
        auto size = static_cast<size_t>(PHYSFS_fileLength(file));
        buffer.resize(size + 1, '\0');
        PHYSFS_readBytes(file, buffer.data(), size);
        PHYSFS_close(file);

        m_filesDataByName[filenameCStr] = buffer;
        return buffer;
    }

    template <>
    std::shared_ptr<AssetMaker> AssetsManager::getMaker<class Texture2D>()
    {
        return std::make_shared<Texture2DMaker>();
    }

    template <>
    std::shared_ptr<AssetMaker> AssetsManager::getMaker<class Shader>()
    {
        return std::make_shared<ShaderMaker>();
    }

    template <>
    std::shared_ptr<AssetMaker> AssetsManager::getMaker<class World>()
    {
        return std::make_shared<WorldMaker>();
    }
}
