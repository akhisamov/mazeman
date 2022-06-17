#include "ResourceManager.hpp"

#include <SDL.h>

ResourceManager::ResourceManager(const std::string& bundleFile)
{
    std::filesystem::path path = SDL_GetBasePath();
    path /= bundleFile;
    BaseResourceManager<Texture2D>::setBundleFile(path);
    BaseResourceManager<Shader>::setBundleFile(path);
}

ResourceManager::~ResourceManager()
{
    BaseResourceManager<Texture2D>::m_resources.clear();
    BaseResourceManager<Shader>::m_resources.clear();
}
