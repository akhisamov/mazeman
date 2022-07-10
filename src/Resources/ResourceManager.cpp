#include "ResourceManager.hpp"
#include "StringUtils.hpp"

#include "SDL_filesystem.h"
#include "SDL_image.h"

#include "physfs.h"

#include<filesystem>

std::shared_ptr<ResourceManager> ResourceManager::create(const std::vector<std::string_view>& searchPaths)
{
    if (PHYSFS_isInit())
    {
        return nullptr;
    }

    constexpr std::string_view errorMessage = "ResourceManager Init Error: %s";
    if (PHYSFS_init(nullptr) == 0)
    {
        throw std::runtime_error(StringUtils::format(errorMessage, PHYSFS_getLastError()));
    }

    for (auto path : searchPaths)
    {
        std::filesystem::path absolutePath = SDL_GetBasePath();
        absolutePath /= path.data();
        if (PHYSFS_mount(absolutePath.string().c_str(), nullptr, 1) == 0)
        {
            throw std::runtime_error(StringUtils::format(errorMessage, PHYSFS_getLastError()));
        }
    }

    return std::shared_ptr<ResourceManager>(new ResourceManager);
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
            throw std::runtime_error(StringUtils::format(message, filename.data(), PHYSFS_getLastError()));
        }
    }
    else
    {
        constexpr std::string_view message = "Resource Load Error [%s]: resource is not found";
        throw std::runtime_error(StringUtils::format(message, filename.data()));
    }
    return buffer;
}