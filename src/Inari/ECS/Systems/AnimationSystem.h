#pragma once

#include "ISystem.h"

namespace inari {
    class AnimationSystem final : public ISystem {
    public:
        AnimationSystem() = default;

    protected:
        void update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;
    };
} // namespace inari
