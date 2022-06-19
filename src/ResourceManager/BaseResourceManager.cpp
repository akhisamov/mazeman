#include "BaseResourceManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "Resources/Texture2D.hpp"
#include "Resources/Shader.hpp"

template<>
BaseResourceManager<Texture2D>::BaseResourceManager(const std::string& bundleFile)
        : BundleReader(bundleFile, ResourceType::TEXTURE2D)
{
}

template<>
const std::shared_ptr<Texture2D>& BaseResourceManager<Texture2D>::baseLoad(const std::string& name)
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
        const std::string message = "Resource Load Error [" + name + "]: texture file is not found";
        throw std::runtime_error(message);
    }

    SDL_Surface* surface = nullptr;
    const std::string data = getFileContent(fileIt->second);
    if (!data.empty())
    {
        SDL_RWops* rw = SDL_RWFromConstMem(data.c_str(), data.size());
        surface = IMG_Load_RW(rw, 1);
        if (surface == nullptr)
        {
            const std::string message = "Resource Load Error [" + name + "]: " + IMG_GetError();
            throw std::runtime_error(message);
        }
    }
    else
    {
        const std::string message = "Resource Load Error [" + name + "]: resource is not found";
        throw std::runtime_error(message);
    }

    const auto& pair = m_resources.emplace(resConfig.id, Texture2D::create(resConfig.id, surface));
    SDL_FreeSurface(surface);
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}

template<>
BaseResourceManager<Shader>::BaseResourceManager(const std::string& bundleFile)
        : BundleReader(bundleFile, ResourceType::SHADER)
{
}

template<>
const std::shared_ptr<Shader>& BaseResourceManager<Shader>::baseLoad(const std::string& name)
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
        const std::string message = "Resource Load Error [" + name + "]: resource is not found";
        throw std::runtime_error(message);
    }

    const auto& pair = m_resources
            .emplace(resourceConfig.id, Shader::create(resourceConfig.id, vertexCode, fragmentCode));
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}
