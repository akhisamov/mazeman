#pragma once

#include <glm/vec4.hpp>

#include "Inari/ECS/Systems/ISystem.hpp"

// Simple AABB collision detection system
class CollisionSystem : public inari::ISystem {
   public:
    explicit CollisionSystem(std::shared_ptr<inari::EntityRegistry> registry);

   protected:
    void update(float dt, const inari::EntityPtr& entity) override;

   private:
    bool check(const inari::EntityPtr& a, const inari::EntityPtr& b) const;
    bool check(const glm::vec4& aRect, const inari::EntityPtr& b) const;
};