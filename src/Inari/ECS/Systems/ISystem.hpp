#pragma once

#include <memory>
#include <utility>

#include "Inari/ECS/EntityRegistry.hpp"

namespace inari {
class ISystem {
   public:
    explicit ISystem(std::shared_ptr<EntityRegistry> registry)
        : m_registry(std::move(registry)) {}
    virtual ~ISystem() = default;

    void updateSystem(float dt) {
        if (m_registry == nullptr) {
            return;
        }

        m_registry->forEachEntity(
            [this, dt](const EntityPtr& entity) { update(dt, entity); });
    }

   protected:
    virtual void update(float dt, const EntityPtr& entity) {}

    const std::shared_ptr<EntityRegistry>& getRegistry() const {
        return m_registry;
    }

   private:
    std::shared_ptr<EntityRegistry> m_registry;
};
}  // namespace inari