#pragma once

#include <memory>
#include <utility>

#include "Inari/ECS/EntityRegistry.hpp"

namespace inari {
    class GameTime;

    class ISystem {
    public:
        explicit ISystem(std::shared_ptr<EntityRegistry> registry)
            : m_registry(std::move(registry))
        {
        }
        virtual ~ISystem() = default;

        void updateSystem(const inari::GameTime& gameTime)
        {
            if (m_registry == nullptr) {
                return;
            }

            m_registry->forEachEntity([this, &gameTime](const EntityPtr& entity) { update(gameTime, entity); });
        }

    protected:
        virtual void update(const inari::GameTime& gameTime, const EntityPtr& entity) { }

        const std::shared_ptr<EntityRegistry>& getRegistry() const { return m_registry; }

    private:
        std::shared_ptr<EntityRegistry> m_registry;
    };
} // namespace inari
