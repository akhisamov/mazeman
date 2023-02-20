#pragma once

#include "Inari/ECS/Systems/ISystem.h"

// Simple AABB collision detection system
class CollisionSystem : public inari::ISystem {
public:
    CollisionSystem() = default;

protected:
    void update(const inari::GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;
};
