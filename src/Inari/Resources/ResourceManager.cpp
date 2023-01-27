#include "ResourceManager.hpp"

#include <SDL_filesystem.h>

#include <physfs.h>

#include <cassert>
#include <iostream>

namespace inari {

    std::shared_ptr<ResourceManager> ResourceManager::create()
    {
        if (PHYSFS_isInit()) {
            return nullptr;
        }

        if (PHYSFS_init(nullptr) == 0) {
            std::cerr << "ResourceManager Init Error: " << PHYSFS_getLastError();
            return nullptr;
        }

        return std::make_shared<ResourceManager>(ResourceManager::Token {});
    }

    void ResourceManager::addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint)
    {
        assert(PHYSFS_isInit());

        std::string absolutePath = SDL_GetBasePath();
        absolutePath += searchPath.data();
        PHYSFS_mount(absolutePath.c_str(), mountPoint.data(), 1);
    }

    void ResourceManager::addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths)
    {
        for (const auto& it : searchPaths) {
            addSearchPath(it.first, it.second);
        }
    }

    void ResourceManager::addFileData(const std::string_view& name, const std::string_view& data)
    {
        m_filesDataByName[name.data()] = data.data();
    }

    void ResourceManager::removeFileData(const std::string_view& name) { m_filesDataByName.erase(name.data()); }

    bool ResourceManager::has(const std::string_view& name)
    {
        const ResourceFindResult result = getResourceByName(name);
        return result.first;
    }

    std::string ResourceManager::readFileData(const std::string_view& filename)
    {
        const char* filenameStr = filename.data();
        auto it = m_filesDataByName.find(filenameStr);
        if (it != m_filesDataByName.end()) {
            return it->second;
        }

        assert(PHYSFS_isInit());
        assert(PHYSFS_exists(filenameStr) != 0);

        PHYSFS_File* file = PHYSFS_openRead(filenameStr);
        assert(file);

        std::string buffer;
        auto size = static_cast<size_t>(PHYSFS_fileLength(file));
        buffer.resize(size + 1, '\0');
        PHYSFS_readBytes(file, buffer.data(), size);
        PHYSFS_close(file);

        addFileData(filename, buffer);
        return buffer;
    }

    ResourceManager::ResourceManager(Token /*unused*/) { }

    ResourceManager::ResourceFindResult ResourceManager::getResourceByName(const std::string_view& name)
    {
        auto it = m_uuidsByName.find(name.data());
        if (it != m_uuidsByName.end()) {
            return getResourceByUUID(it->second);
        }
        return std::make_pair(false, nullptr);
    }

    ResourceManager::ResourceFindResult ResourceManager::getResourceByUUID(const ResourceUUID& uuid)
    {
        auto it = m_resourcesByUuid.find(uuid);
        if (it != m_resourcesByUuid.end()) {
            return std::make_pair(true, it->second);
        }
        return std::make_pair(false, nullptr);
    }
} // namespace inari
