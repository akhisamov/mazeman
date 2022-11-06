#pragma once

#include "ISystem.hpp"

namespace inari {
class AnimationSystem final : public ISystem {
   public:
    explicit AnimationSystem(std::shared_ptr<EntityRegistry> registry);

   protected:
    void update(float dt, const EntityPtr& entity) override;
};
}  // namespace inari