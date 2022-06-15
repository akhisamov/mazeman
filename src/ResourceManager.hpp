#pragma once

#include <memory>
#include <filesystem>
#include <map>
#include <functional>

class ResourceManager
{
public:
    explicit ResourceManager(const std::string& bundleFile);

    template<typename T>
    std::shared_ptr<T> load(const std::string& name)
    {
        return nullptr;
    }

    bool unload(const std::string& name);

private:
    std::map<std::string, std::string> readBundle(const std::string& viewName, const std::string& resourceName, int& resourceId);

private:
    std::filesystem::path m_bundleFile;

    std::map<std::string, std::shared_ptr<class Resource>> m_resources;
};

template<>
std::shared_ptr<class Texture2D> ResourceManager::load(const std::string& name);
template<>
std::shared_ptr<class Shader> ResourceManager::load(const std::string& name);