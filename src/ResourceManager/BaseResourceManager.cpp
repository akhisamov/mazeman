#include "BaseResourceManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "Texture2D.hpp"
#include "Shader.hpp"

template<>
const std::shared_ptr<Texture2D>& BaseResourceManager<Texture2D>::baseLoad(const std::string& name)
{
    const auto& it = m_resources.find(name);
    if (it != m_resources.end())
    {
        return it->second;
    }

    int resourceId = 0;
    SDL_Surface* surface = nullptr;

    const auto data = readBundle("textures_v", name, resourceId);
    auto dataIt = data.find("file");
    if (dataIt != data.end())
    {
        SDL_RWops* rw = SDL_RWFromConstMem(dataIt->second.c_str(), dataIt->second.size());
        surface = IMG_Load_RW(rw, 1);
        if (surface == nullptr)
        {
            const std::string message = "Resource Load Error [" + name + "]: " + IMG_GetError();
            throw std::runtime_error(message);
        }
    }

    const auto& pair = m_resources.emplace(name, Texture2D::create(resourceId, surface));
    SDL_FreeSurface(surface);
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}

template<>
const std::shared_ptr<Shader>& BaseResourceManager<Shader>::baseLoad(const std::string& name)
{
    const auto& it = m_resources.find(name);
    if (it != m_resources.end())
    {
        return it->second;
    }

    std::string vertexCode;
    std::string fragmentCode;
    int resourceId = 0;

    const auto data = readBundle("shaders_v", name, resourceId);
    if (!data.empty())
    {
        auto vertexIt = data.find("vertex");
        auto fragmentIt = data.find("fragment");
        if (vertexIt != data.end() && fragmentIt != data.end())
        {
            vertexCode = vertexIt->second;
            fragmentCode = fragmentIt->second;
        }
    }

    if (data.empty() || fragmentCode.empty() || vertexCode.empty())
    {
        const std::string message = "Resource Load Error [" + name + "]: resource is not found";
        throw std::runtime_error(message);
    }

    const auto& pair = m_resources.emplace(name, Shader::create(resourceId, vertexCode, fragmentCode));
    if (!pair.second)
    {
        const std::string message = "Resource Load Error [" + name + "]: can not save resource";
        throw std::runtime_error(message);
    }
    return pair.first->second;
}
