#pragma once

#include "ISystem.hpp"

namespace inari {
class PhysicsSystem : public ISystem {
   public:
    explicit PhysicsSystem(std::shared_ptr<EntityRegistry> registry);

    void update(float dt);
};
}  // namespace inari