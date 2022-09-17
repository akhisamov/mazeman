#pragma once

#include <any>
#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace inari {
struct Entity {
    std::string uuid;
    std::string name;
};
using EntityPtr = std::shared_ptr<Entity>;

class EntityRegistry {
    using ComponentHash = size_t;
    using AnyComponent = std::any;
    using ComponentMap = std::map<ComponentHash, AnyComponent>;

   public:
    EntityRegistry() = default;
    ~EntityRegistry() = default;

    EntityPtr createEntity(const std::string_view& name = "");

    EntityPtr getEntity(const std::string_view& name);
    const std::vector<EntityPtr>& getEntities() const { return m_entities; }

    bool destroyEntity(const EntityPtr& entity);
    bool destroyEntity(const std::string_view& name);

    template <class C, class... Args>
    C* emplaceComponent(const EntityPtr& entity, Args... args) {
        assert(entity != nullptr && "Entity is empty");

        auto& componentMap = m_components[entity->uuid];
        ComponentHash hashCode = typeid(C).hash_code();
        if (componentMap.find(hashCode) == componentMap.end()) {
            auto resultPair =
                componentMap.emplace(hashCode, std::any(C{args...}));
            if (resultPair.second) {
                return std::any_cast<C>(&resultPair.first->second);
            }
        }
        return nullptr;
    }

    template <class C>
    C* getComponent(const EntityPtr& entity) {
        assert(entity != nullptr && "Entity is empty");

        const auto& entityIt = m_components.find(entity->uuid);
        if (entityIt != m_components.end()) {
            const auto& componentIt =
                entityIt->second.find(typeid(C).hash_code());
            if (componentIt != entityIt->second.end()) {
                return std::any_cast<C>(&componentIt->second);
            }
        }

        return nullptr;
    }

   private:
    std::vector<EntityPtr> m_entities;
    std::map<std::string, ComponentMap> m_components;
};
}  // namespace inari