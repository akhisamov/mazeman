#pragma once

#include "Resources/Texture2D.hpp"
#include "Resources/Shader.hpp"

#include "BaseResourceManager.hpp"

#include <type_traits>

class ResourceManager final :
        BaseResourceManager<Texture2D>,
        BaseResourceManager<Shader>
{
public:
    explicit ResourceManager(const std::string_view& bundleFile);
    ~ResourceManager() override;

    template<class T>
    const std::shared_ptr<T>& load(const std::string_view& name)
    {
        static_assert(std::is_base_of_v<Resource, T>);
        return BaseResourceManager<T>::baseLoad(name);
    }

    template<class T>
    bool unload(const std::string_view& name)
    {
        static_assert(std::is_base_of_v<Resource, T>);
        return BaseResourceManager<T>::baseUnload(name);
    }

    template<class T>
    bool unload(const std::shared_ptr<T>& resource)
    {
        static_assert(std::is_base_of_v<Resource, T>);
        return BaseResourceManager<T>::m_resources.erase(resource->getResourceId()) == 1;
    }
};
