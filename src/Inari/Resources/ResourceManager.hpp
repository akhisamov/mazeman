#pragma once

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "Inari/Resources/IResource.hpp"

namespace inari {
class ResourceManager final {
    using ResourceName = std::string;
    using ResourceUUID = std::string;
    using ResourceHashCode = size_t;
    using ResourceFindResult = std::pair<bool, std::shared_ptr<IResource>>;

   protected:
    struct Token {};

   public:
    static std::shared_ptr<ResourceManager> create();

    explicit ResourceManager(Token);
    ResourceManager() = delete;

    void addSearchPath(const std::string_view& searchPath,
                       const std::string_view& mountPoint = "");
    void addSearchPaths(
        const std::map<std::string_view, std::string_view>& searchPaths);

    template <class T>
    std::shared_ptr<T> load(const std::string_view& name) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        ResourceFindResult result = getResourceByName<T>(name);
        if (result.first) {
            return std::dynamic_pointer_cast<T>(result.second);
        }

        std::shared_ptr<T> resource = T::createFromData(readFileData(name));
        if (resource == nullptr) {
            return nullptr;
        }

        const ResourceHashCode hashCode = typeid(T).hash_code();
        const ResourceUUID& uuid = resource->getUUID();
        m_names[hashCode].emplace(name, uuid);
        m_resources.emplace(uuid, resource);

        return resource;
    }

    template <class T, class... Args>
    std::shared_ptr<T> loadOrCreate(const std::string_view& name,
                                    Args... args) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        ResourceFindResult result = getResourceByName<T>(name);
        if (result.first) {
            return std::dynamic_pointer_cast<T>(result.second);
        }

        std::shared_ptr<T> resource = T::create(args...);
        if (resource == nullptr) {
            return nullptr;
        }

        const ResourceHashCode hashCode = typeid(T).hash_code();
        const ResourceUUID& uuid = resource->getUUID();
        m_names[hashCode].emplace(name, uuid);
        m_resources.emplace(uuid, resource);

        return resource;
    }

    template <class T>
    bool unload(const std::string_view& name) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        const ResourceHashCode hashCode = typeid(T).hash_code();

        ResourceUUID uuid;
        auto it = m_names.find(hashCode);
        if (it != m_names.end()) {
            auto& uuids = it->second;
            auto uuidIt = uuids.find(name.data());
            if (uuidIt != uuids.end()) {
                uuid = uuidIt->second;
                uuids.erase(uuidIt);
            }
        }

        if (uuid.empty()) {
            return false;
        }

        return m_resources.erase(uuid) == 1;
    }

    template <class T>
    bool has(const std::string_view& name) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        const ResourceFindResult result = getResourceByName<T>(name);
        return result.first;
    }

   private:
    static std::string readFileData(const std::string_view& filename);

    template <class T>
    ResourceFindResult getResourceByName(const std::string_view& name) {
        const ResourceHashCode hashCode = typeid(T).hash_code();
        auto it = m_names.find(hashCode);
        if (it != m_names.end()) {
            const auto& uuids = it->second;
            auto uuidIt = uuids.find(name.data());
            if (uuidIt != uuids.end()) {
                return getResourceByUUID(uuidIt->second);
            }
        }
        return std::make_pair(false, nullptr);
    }
    ResourceFindResult getResourceByUUID(const ResourceUUID& uuid);

    std::map<ResourceHashCode, std::map<ResourceName, ResourceUUID>> m_names;
    std::map<ResourceUUID, std::shared_ptr<IResource>> m_resources;
};
}  // namespace inari