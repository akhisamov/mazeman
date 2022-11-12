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

    void addFileData(const std::string_view& name,
                     const std::string_view& data);
    void removeFileData(const std::string_view& name);

    template <class T>
    std::shared_ptr<T> load(const std::string_view& name) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        ResourceFindResult result = getResourceByName(name);
        if (result.first) {
            return std::dynamic_pointer_cast<T>(result.second);
        }

        std::shared_ptr<T> resource = T::createFromData(readFileData(name));
        if (resource == nullptr) {
            return nullptr;
        }

        const ResourceUUID& uuid = resource->getUUID();
        m_uuidsByName.emplace(name.data(), uuid);
        m_resourcesByUuid.emplace(uuid, resource);

        return resource;
    }

    template <class T>
    bool unload(const std::string_view& name) {
        static_assert(std::is_base_of_v<IResource, T>, "Is not resource");

        const ResourceHashCode hashCode = typeid(T).hash_code();

        ResourceUUID uuid;
        auto it = m_uuidsByName.find(name.data());
        if (it != m_uuidsByName.end()) {
            uuid = it->second;
            m_uuidsByName.erase(it);
        }

        if (uuid.empty()) {
            return false;
        }

        return m_resourcesByUuid.erase(uuid) == 1;
    }

    bool has(const std::string_view& name);

   private:
    std::string readFileData(const std::string_view& filename);

    ResourceFindResult getResourceByName(const std::string_view& name);
    ResourceFindResult getResourceByUUID(const ResourceUUID& uuid);

    std::map<ResourceName, std::string> m_filesDataByName;
    std::map<ResourceName, ResourceUUID> m_uuidsByName;
    std::map<ResourceUUID, std::shared_ptr<IResource>> m_resourcesByUuid;
};
}  // namespace inari
