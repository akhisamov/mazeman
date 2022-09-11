#pragma once

#include "ISystem.hpp"

namespace inari
{
    class AnimationSystem final : public ISystem
    {
    public:
        AnimationSystem(const std::shared_ptr<EntityRegistry>& registry);

        void update(float dt) override;
    };
}