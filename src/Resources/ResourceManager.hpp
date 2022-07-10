#pragma once

#include <map>
#include <memory>
#include <vector>

#include "StringUtils.hpp"

class ResourceManager
{
public:
    static std::shared_ptr<ResourceManager> create(const std::vector<std::string_view>& searchPaths);
    ~ResourceManager() = default;

    template <class T>
    const std::shared_ptr<T>& load(const std::string_view& name)
    {
        auto& resources = getResources<T>();
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
        return getResources<T>().erase(name) == 1;
    }

private:
    ResourceManager() = default;

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

    // TODO change it to ResourceStorage<T> class
    template <class T>
    std::map<std::string_view, std::shared_ptr<T>>& getResources()
    {
        static std::map<std::string_view, std::shared_ptr<T>> resources;
        return resources;
    }
};
