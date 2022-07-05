#include "BaseResourceManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Resources/Texture2D.hpp"
#include "Resources/Shader.hpp"

#include "StringUtils.hpp"

template<>
BaseResourceManager<Texture2D>::BaseResourceManager(const std::string_view& bundleFile)
        : BundleReader(bundleFile, ResourceType::TEXTURE2D)
{
}

template<>
const std::shared_ptr<Texture2D>& BaseResourceManager<Texture2D>::baseLoad(const std::string_view& name)
{
    const BundleResource& resConfig = getResourceConfig(name);

    const auto& resIt = m_resources.find(resConfig.id);
    if (resIt != m_resources.end())
    {
        return resIt->second;
    }

    const auto& fileIt = resConfig.data.find("file");
    if (fileIt == resConfig.data.end())
    {
        const std::string_view message = "Resource Load Error [%s]: texture file is not found";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }

    SDL_Surface* surface = nullptr;
    const std::string data = getFileContent(fileIt->second);
    if (!data.empty())
    {
        SDL_RWops* rw = SDL_RWFromConstMem(data.c_str(), data.size());
        surface = IMG_Load_RW(rw, 1);
        if (surface == nullptr)
        {
            const std::string_view message = "Resource Load Error [%s]: %s";
            throw std::runtime_error(StringUtils::format(message, name.data(), IMG_GetError()));
        }
    }
    else
    {
        const std::string_view message = "Resource Load Error [%s]: resource is not found";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }

    const auto& pair = m_resources.emplace(resConfig.id, Texture2D::create(resConfig.id, surface));
    SDL_FreeSurface(surface);
    if (!pair.second)
    {
        const std::string_view message = "Resource Load Error [%s]: can not save resource";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }
    return pair.first->second;
}

template<>
BaseResourceManager<Shader>::BaseResourceManager(const std::string_view& bundleFile)
        : BundleReader(bundleFile, ResourceType::SHADER)
{
}

template<>
const std::shared_ptr<Shader>& BaseResourceManager<Shader>::baseLoad(const std::string_view& name)
{
    const BundleResource& resourceConfig = getResourceConfig(name);

    const auto& resIt = m_resources.find(resourceConfig.id);
    if (resIt != m_resources.end())
    {
        return resIt->second;
    }

    std::string vertexCode;
    std::string fragmentCode;
    for (const auto& it: resourceConfig.data)
    {
        if (it.first == "vertex")
        {
            vertexCode = getFileContent(it.second);
        }
        else if (it.first == "fragment")
        {
            fragmentCode = getFileContent(it.second);
        }
    }

    if (vertexCode.empty() || fragmentCode.empty())
    {
        const std::string_view message = "Resource Load Error [%s]: resource is not found";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }

    const auto& pair = m_resources
            .emplace(resourceConfig.id, Shader::create(resourceConfig.id, vertexCode, fragmentCode));
    if (!pair.second)
    {
        const std::string_view message = "Resource Load Error [%s]: can not save resource";
        throw std::runtime_error(StringUtils::format(message, name.data()));
    }
    return pair.first->second;
}
