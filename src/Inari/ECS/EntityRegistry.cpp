#include "EntityRegistry.hpp"

#include <algorithm>

struct NameComparator {
public:
    explicit NameComparator(const std::string_view& name)
        : m_name(name)
    {
    }

    bool operator()(const inari::EntityPtr& entity) { return entity && entity->name == m_name; }

private:
    std::string m_name;
};

namespace inari {
    Entity::Entity(const std::string_view& name)
        : name(name)
        , id([] {
            static Entity::ID idCounter = 0;
            return idCounter++;
        }())
    {
    }

    EntityPtr EntityRegistry::createEntity(const std::string_view& name)
    {
        EntityPtr entity = std::make_shared<Entity>(name);
        m_entities.push_back(entity);
        return entity;
    }

    EntityPtr EntityRegistry::getEntity(const std::string_view& name)
    {
        if (!name.empty()) {
            auto it = std::find_if(m_entities.begin(), m_entities.end(), NameComparator(name));
            if (it != m_entities.end()) {
                return *it;
            }
        }
        return nullptr;
    }

    void EntityRegistry::forEachEntity(const VoidHandler& handler) const
    {
        if (!handler) {
            return;
        }

        const auto callback = [handler](const EntityPtr& entity) {
            if (entity == nullptr) {
                return;
            }
            handler(entity);
        };
        std::for_each(m_entities.begin(), m_entities.end(), callback);
    }

    bool EntityRegistry::anyOfEntity(const BoolHandler& handler)
    {
        if (!handler) {
            return false;
        }

        const auto callback = [handler](const EntityPtr& entity) {
            if (entity == nullptr) {
                return false;
            }
            return handler(entity);
        };
        return std::any_of(m_entities.begin(), m_entities.end(), callback);
    }

    EntityPtr EntityRegistry::findEntity(const BoolHandler& handler) const
    {
        if (!handler) {
            return nullptr;
        }

        const auto callback = [handler](const EntityPtr& entity) {
            if (entity == nullptr) {
                return false;
            }
            return handler(entity);
        };
        const auto it = std::find_if(m_entities.begin(), m_entities.end(), callback);
        if (it == m_entities.end()) {
            return nullptr;
        }
        return *it;
    }

    bool EntityRegistry::destroyEntity(const EntityPtr& entity)
    {
        assert(entity != nullptr && "Entity is empty");

        const bool result = m_components.erase(entity->id);
        const auto it = std::find(m_entities.begin(), m_entities.end(), entity);
        if (it != m_entities.end()) {
            m_entities.erase(it);
            return result;
        }
        return result;
    }

    bool EntityRegistry::destroyEntity(const std::string_view& name)
    {
        if (!name.empty()) {
            const auto it = std::find_if(m_entities.begin(), m_entities.end(), NameComparator(name));
            if (it != m_entities.end()) {
                const EntityPtr& entity = *it;
                assert(entity != nullptr && "Entity is empty");
                const bool result = m_components.erase(entity->id);
                m_entities.erase(it);
                return result;
            }
        }

        return false;
    }
} // namespace inari
