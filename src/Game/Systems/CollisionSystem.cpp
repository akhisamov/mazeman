#include "CollisionSystem.hpp"

#include <glm/geometric.hpp>

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Inari/Utils/GameTime.hpp"

#include "Game/Components/Collision.hpp"

using namespace inari;

namespace {
    struct AABB {
        using DeltaPair = std::pair<glm::vec2, glm::vec2>;

        glm::vec2 min;
        glm::vec2 max;

        explicit AABB(const glm::vec4& rect)
            : min(rect.x, rect.y)
            , max(min + glm::vec2(rect.z, rect.w))
        {
        }

        static DeltaPair calculateDeltas(const AABB& a, const AABB& b)
        {
            const glm::vec2 first(b.min.x - a.max.x, b.min.y - a.max.y);
            const glm::vec2 second(a.min.x - b.max.x, a.min.y - b.max.y);
            return { first, second };
        }

        static bool intersect(const AABB& a, const AABB& b)
        {
            const DeltaPair deltas = calculateDeltas(a, b);
            return deltas.first.x < 0.0f && deltas.first.y < 0.0f && deltas.second.x < 0.0f && deltas.second.y < 0.0f;
        }
    };

    void updateVelocity(float dt, glm::vec2& velocity, const AABB& a, const AABB& b)
    {
        auto deltas = AABB::calculateDeltas(a, b);

        // check direction
        // x-axis
        if (velocity.x > 0 && deltas.first.x >= 0) { // right
            velocity.x = deltas.first.x / dt;
        } else if (velocity.x < 0 && deltas.second.x >= 0) { // left
            velocity.x = -deltas.second.x / dt;
        }
        // y-axis
        if (velocity.y > 0 && deltas.first.y >= 0) { // down
            velocity.y = deltas.first.y / dt;
        } else if (velocity.y < 0 && deltas.second.y >= 0) { // up
            velocity.y = -deltas.second.y / dt;
        }
    }
} // namespace

CollisionSystem::CollisionSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry))
{
}

void CollisionSystem::update(const GameTime& gameTime, const EntityPtr& entity)
{
    const auto* collision = getRegistry()->getComponent<Collision>(entity);
    if (collision == nullptr || !collision->isDynamic) {
        return;
    }

    if (!getRegistry()->hasComponent<RigidBody>(entity)) {
        return;
    }

    const auto* transform = getRegistry()->getComponent<Transform>(entity);
    auto* rigidBody = getRegistry()->getComponent<RigidBody>(entity);
    if (transform == nullptr || rigidBody == nullptr) {
        return;
    }

    if (rigidBody->velocity == glm::vec2(0)) {
        return;
    }

    const glm::vec4 futureRect(transform->getAbsolutePosition() + (rigidBody->velocity * gameTime.getElapsedTime()),
                               transform->size);
    const auto callback = [this, entity, aRect = futureRect](const EntityPtr& compareWith) {
        if (entity == compareWith) {
            return false;
        }

        if (!getRegistry()->hasComponent<Collision>(compareWith)) {
            return false;
        }

        const auto* transform = getRegistry()->getComponent<Transform>(compareWith);
        if (transform == nullptr) {
            return false;
        }

        const AABB a(aRect);
        const AABB b(transform->getRect());
        return AABB::intersect(a, b);
    };
    const EntityPtr collidedEntity = getRegistry()->findEntity(callback);
    if (collidedEntity == nullptr) {
        return;
    }

    const auto* collidedTransform = getRegistry()->getComponent<Transform>(collidedEntity);
    if (collidedTransform == nullptr) {
        return;
    }

    const AABB a(transform->getRect());
    const AABB b(collidedTransform->getRect());
    updateVelocity(gameTime.getElapsedTime(), rigidBody->velocity, a, b);
}
