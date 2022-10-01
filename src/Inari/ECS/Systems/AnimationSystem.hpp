#pragma once

#include "ISystem.hpp"

namespace inari {
class AnimationSystem final : public ISystem {
   public:
    explicit AnimationSystem(std::shared_ptr<EntityRegistry> registry);

    void update(float dt) override;
};
}  // namespace inari