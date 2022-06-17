#pragma once

#include <memory>
#include <filesystem>
#include <map>
#include <functional>

class Texture2D;
class Shader;

class ResourceManager
{
public:
    explicit ResourceManager(const std::string& bundleFile);

    template<class T>
    const std::shared_ptr<T>& load(const std::string& name);

    template<class T>
    bool unload(const std::string& name);

private:
    std::map<std::string, std::string> readBundle(const std::string& viewName, const std::string& resourceName, int& resourceId);

    std::filesystem::path m_bundleFile;

    std::map<std::string, std::shared_ptr<Texture2D>> m_textures;
    std::map<std::string, std::shared_ptr<Shader>> m_shader;
};

template<>
const std::shared_ptr<Texture2D>& ResourceManager::load(const std::string& name);
template<>
const std::shared_ptr<Shader>& ResourceManager::load(const std::string& name);

template<>
bool ResourceManager::unload<Texture2D>(const std::string& name);
template<>
bool ResourceManager::unload<Shader>(const std::string& name);
