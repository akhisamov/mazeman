#include "ResourceManager.hpp"

#include <SDL_filesystem.h>
#include <SDL_image.h>

#include <physfs.h>

#include <filesystem>

#include "Utils/Strings.hpp"

std::shared_ptr<ResourceManager> ResourceManager::create()
{
    if (PHYSFS_isInit())
    {
        return nullptr;
    }

    if (PHYSFS_init(nullptr) == 0)
    {
        constexpr std::string_view message = "ResourceManager Init Error: %s";
        throw std::runtime_error(strings::format(message, PHYSFS_getLastError()));
    }

    return std::make_shared<ResourceManager>(ResourceManager::Token {});
}

void ResourceManager::addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint)
{
    std::filesystem::path absolutePath = SDL_GetBasePath();
    absolutePath /= searchPath.data();
    if (PHYSFS_mount(absolutePath.string().c_str(), mountPoint.data(), 1) == 0)
    {
        constexpr std::string_view message = "ResourceManager::addSearchPath Error: %s";
        throw std::runtime_error(strings::format(message, PHYSFS_getLastError()));
    }
}

void ResourceManager::addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths)
{
    for (auto it : searchPaths)
    {
        addSearchPath(it.first, it.second);
    }
}

std::string ResourceManager::readFileData(const std::string_view& filename)
{
    std::string buffer;
    if (PHYSFS_exists(filename.data()) != 0)
    {
        PHYSFS_File* file = PHYSFS_openRead(filename.data());
        if (file != nullptr)
        {
            auto size = static_cast<size_t>(PHYSFS_fileLength(file));
            buffer.resize(size + 1, '\0');
            PHYSFS_readBytes(file, buffer.data(), size);
            PHYSFS_close(file);
        }
        else
        {
            constexpr std::string_view message = "Resource Load Error [%s]: %s";
            throw std::runtime_error(strings::format(message, filename.data(), PHYSFS_getLastError()));
        }
    }
    else
    {
        constexpr std::string_view message = "Resource Load Error [%s]: resource is not found";
        throw std::runtime_error(strings::format(message, filename.data()));
    }
    return buffer;
}

ResourceManager::ResourceManager(Token) { }
