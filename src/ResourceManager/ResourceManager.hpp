#pragma once

#include "BaseResourceManager.hpp"
#include "Texture2D.hpp"
#include "Shader.hpp"

class ResourceManager final :
        BaseResourceManager<Texture2D>,
        BaseResourceManager<Shader>
{
public:
    explicit ResourceManager(const std::string& bundleFile);
    ~ResourceManager() override;

    template<class T>
    const std::shared_ptr<T>& load(const std::string& name)
    {
        return BaseResourceManager<T>::baseLoad(name);
    }

    template<class T>
    bool unload(const std::string& name)
    {
        return BaseResourceManager<T>::baseUnload(name);
    }
};
