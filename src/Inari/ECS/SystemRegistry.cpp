#include "SystemRegistry.h"

#include "Inari/ECS/EntityRegistry.h"

namespace inari {
    void SystemRegistry::updateOrderedSystem(const GameTime& gameTime, const EntityRegistryPtr& entityRegistry)
    {
        for (const SystemHash& hash : m_order) {
            const auto& it = m_systems.find(hash);
            if (it != m_systems.end()) {
                updateSystem(it->second, gameTime, entityRegistry);
            }
        }
    }

    void SystemRegistry::updateSystem(const std::shared_ptr<ISystem>& system, const GameTime& gameTime,
                                      const EntityRegistryPtr& entityRegistry)
    {
        assert(system != nullptr && "System is empty");
        assert(entityRegistry != nullptr && "Entity Registry is empty");

        for (const EntityPtr& entity : entityRegistry->getEntities()) {
            system->update(gameTime, entityRegistry, entity);
        }
    }
}