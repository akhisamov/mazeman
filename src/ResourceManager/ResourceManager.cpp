#include "ResourceManager.hpp"

#include <SDL2/SDL.h>

ResourceManager::ResourceManager(const std::string_view& bundleFile)
        :
        BaseResourceManager<Texture2D>(bundleFile),
        BaseResourceManager<Shader>(bundleFile)
{
}

ResourceManager::~ResourceManager()
{
    BaseResourceManager<Texture2D>::m_resources.clear();
    BaseResourceManager<Shader>::m_resources.clear();
}
