#include "EntityRegistry.hpp"

#include "Inari/Utils/Random.hpp"

struct NameComparator {
   public:
    explicit NameComparator(const std::string_view& name) : m_name(name) {}

    bool operator()(const inari::EntityPtr& entity) {
        return entity ? entity->name == m_name : false;
    }

   private:
    std::string m_name;
};

namespace inari {
EntityPtr EntityRegistry::createEntity(const std::string_view& name) {
    EntityPtr entity = std::make_shared<Entity>();
    entity->uuid = random::generateUUID();
    entity->name = name;
    m_entities.push_back(entity);
    return entity;
}

EntityPtr EntityRegistry::getEntity(const std::string_view& name) {
    if (!name.empty()) {
        auto it = std::find_if(m_entities.begin(), m_entities.end(),
                               NameComparator(name));
        if (it != m_entities.end()) {
            return *it;
        }
    }
    return nullptr;
}

bool EntityRegistry::destroyEntity(const EntityPtr& entity) {
    assert(entity != nullptr && "Entity is empty");

    const bool result = m_components.erase(entity->uuid);
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end()) {
        m_entities.erase(it);
        return result;
    }
    return result;
}

bool EntityRegistry::destroyEntity(const std::string_view& name) {
    if (!name.empty()) {
        auto it = std::find_if(m_entities.begin(), m_entities.end(),
                               NameComparator(name));
        if (it != m_entities.end()) {
            return destroyEntity(*it);
        }
    }

    return false;
}
}  // namespace inari