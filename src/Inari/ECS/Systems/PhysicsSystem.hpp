#pragma once

#include "ISystem.hpp"

namespace inari {
    class PhysicsSystem : public ISystem {
    public:
        explicit PhysicsSystem(std::shared_ptr<EntityRegistry> registry);

    protected:
        void update(const inari::GameTime& gameTime, const EntityPtr& entity) override;
    };
} // namespace inari
