#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace inari {
    struct Entity {
        using ID = uint64_t;
        const ID id;
        std::string name;

        explicit Entity(const std::string_view& name);
    };
    using EntityPtr = std::shared_ptr<Entity>;

    class EntityRegistry final {
        friend class SystemRegistry;

        using ComponentHash = size_t;
        using AnyComponent = std::any;
        using ComponentMap = std::map<ComponentHash, AnyComponent>;
        using BoolHandler = std::function<bool(const EntityPtr&)>;

    public:
        EntityRegistry() = default;
        ~EntityRegistry() = default;

        EntityPtr createEntity(const std::string_view& name = "");

        EntityPtr getEntity(const std::string_view& name);

        bool anyOfEntity(const BoolHandler& handler);
        EntityPtr findEntity(const BoolHandler& handler) const;

        bool destroyEntity(const EntityPtr& entity);
        bool destroyEntity(const std::string_view& name);

        template <class C>
        void emplaceComponent(const EntityPtr& entity)
        {
            emplaceComponent(entity, C {});
        }

        template <class C>
        void emplaceComponent(const EntityPtr& entity, const C& component)
        {
            assert(entity != nullptr && "Entity is empty");
            auto& componentMap = m_components[entity->id];
            ComponentHash hashCode = typeid(C).hash_code();
            if (componentMap.find(hashCode) == componentMap.end()) {
                auto resultPair = componentMap.emplace(hashCode, std::any(component));
            }
        }

        template <class C>
        C* getComponent(const EntityPtr& entity)
        {
            assert(entity != nullptr && "Entity is empty");
            const auto& entityIt = m_components.find(entity->id);
            if (entityIt != m_components.end()) {
                const auto& componentIt = entityIt->second.find(typeid(C).hash_code());
                if (componentIt != entityIt->second.end()) {
                    return std::any_cast<C>(&componentIt->second);
                }
            }
            return nullptr;
        }

        template <class C>
        bool hasComponent(const EntityPtr& entity) const
        {
            assert(entity != nullptr && "Entity is empty");
            const auto& entityIt = m_components.find(entity->id);
            if (entityIt != m_components.end()) {
                const auto& componentIt = entityIt->second.find(typeid(C).hash_code());
                return componentIt != entityIt->second.end();
            }
            return false;
        }

    protected:
        const std::vector<EntityPtr>& getEntities() const { return m_entities; }

    private:
        std::vector<EntityPtr> m_entities;
        std::map<Entity::ID, ComponentMap> m_components;
    };
} // namespace inari
