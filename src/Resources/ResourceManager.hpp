#pragma once

#include <vector>

#include "Resources/ResourceStorage.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture2D.hpp"

#include "StringUtils.hpp"

class ResourceManager final
    : public ResourceStorage<Texture2D>
    , public ResourceStorage<Shader>
{
protected:
    struct Token
    {
    };

public:
    static std::shared_ptr<ResourceManager> create(const std::vector<std::string_view>& searchPaths);

    explicit ResourceManager(Token);
    ResourceManager() = delete;

    template <class T>
    const std::shared_ptr<T>& load(const std::string_view& name)
    {
        auto& resources = ResourceStorage<T>::getResources();
        if (!resources.empty())
        {
            const auto& it = resources.find(name);
            if (it != resources.end())
            {
                return it->second;
            }
        }

        const auto& pair = resources.emplace(name, T::createFromData(getData<T>(name)));
        if (!pair.second)
        {
            constexpr std::string_view message = "Resource Load Error [%s]: can not save resource";
            throw std::runtime_error(StringUtils::format(message, name.data()));
        }
        return pair.first->second;
    }

    template <class T>
    bool unload(const std::string_view& name)
    {
        return ResourceStorage<T>::getResources().erase(name) == 1;
    }

private:
    static std::string readFileData(const std::string_view& filename);

    template <typename T>
    std::map<std::string, std::string> getData(const std::string_view& name)
    {
        std::map<std::string, std::string> result;

        for (auto filename : T::getFiles(name))
        {
            const std::string data = readFileData(filename);
            if (!data.empty())
            {
                result.emplace(filename, data);
            }
        }

        return result;
    }
};
