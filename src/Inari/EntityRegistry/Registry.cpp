#include "Registry.hpp"

#include "Inari/Utils/Random.hpp"

namespace inari
{
    const EntityPtr& Registry::create(const std::string_view& name)
    {
        const EntityPtr& entity = m_collection.emplace(std::make_shared<Entity>(), ComponentMap {}).first->first;
        entity->uuid = random::generateUUID();
        entity->name = name;
        return entity;
    }

    bool Registry::destroy(const EntityPtr& entity) { return m_collection.erase(entity) != 0; }
    bool Registry::destroyByName(const std::string_view& name)
    {
        if (!name.empty())
        {
            auto it = std::find_if(m_collection.begin(), m_collection.end(),
                                   [&name](const auto& pair) { return pair.first->name == name; });
            if (it != m_collection.end())
            {
                m_collection.erase(it);
                return true;
            }
        }

        return false;
    }
}