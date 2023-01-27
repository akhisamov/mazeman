#pragma once

#include "ISystem.hpp"

namespace inari {
    class AnimationSystem final : public ISystem {
    public:
        explicit AnimationSystem(std::shared_ptr<EntityRegistry> registry);

    protected:
        void update(const inari::GameTime& gameTime, const EntityPtr& entity) override;
    };
} // namespace inari
