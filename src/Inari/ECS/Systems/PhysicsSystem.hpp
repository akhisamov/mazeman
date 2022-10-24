#pragma once

#include "ISystem.hpp"

namespace inari {
class PhysicsSystem : public ISystem {
   public:
    explicit PhysicsSystem(std::shared_ptr<EntityRegistry> registry);

   protected:
    void update(float dt, const EntityPtr& entity) override;
};
}  // namespace inari