#pragma once

#include "ISystem.h"

namespace inari {
    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem() = default;

    protected:
        void update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;
    };
} // namespace inari
