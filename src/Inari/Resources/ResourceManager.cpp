#include "ResourceManager.hpp"

#include <SDL_filesystem.h>

#include <physfs.h>

#include <cassert>
#include <iostream>

namespace inari {

std::shared_ptr<ResourceManager> ResourceManager::create() {
    if (PHYSFS_isInit()) {
        return nullptr;
    }

    if (PHYSFS_init(nullptr) == 0) {
        std::cerr << "ResourceManager Init Error: " << PHYSFS_getLastError();
        return nullptr;
    }

    return std::make_shared<ResourceManager>(ResourceManager::Token{});
}

void ResourceManager::addSearchPath(const std::string_view& searchPath,
                                    const std::string_view& mountPoint) {
    assert(PHYSFS_isInit());

    std::string absolutePath = SDL_GetBasePath();
    absolutePath += searchPath.data();
    PHYSFS_mount(absolutePath.c_str(), mountPoint.data(), 1);
}

void ResourceManager::addSearchPaths(
    const std::map<std::string_view, std::string_view>& searchPaths) {
    for (const auto& it : searchPaths) {
        addSearchPath(it.first, it.second);
    }
}

std::string ResourceManager::readFileData(const std::string_view& filename) {
    assert(PHYSFS_isInit());
    assert(PHYSFS_exists(filename.data()) != 0);

    PHYSFS_File* file = PHYSFS_openRead(filename.data());
    assert(file);

    std::string buffer;
    auto size = static_cast<size_t>(PHYSFS_fileLength(file));
    buffer.resize(size + 1, '\0');
    PHYSFS_readBytes(file, buffer.data(), size);
    PHYSFS_close(file);
    return buffer;
}

ResourceManager::ResourceManager(Token /*unused*/) {}

ResourceManager::ResourceFindResult ResourceManager::getResourceByUUID(
    const ResourceUUID& uuid) {
    auto it = m_resources.find(uuid);
    if (it != m_resources.end()) {
        return std::make_pair(true, it->second);
    }
    return std::make_pair(false, nullptr);
}
}  // namespace inari
