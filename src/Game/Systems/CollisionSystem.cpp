#include "CollisionSystem.hpp"

#include <glm/geometric.hpp>

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Game/Components/Collision.hpp"

namespace {
struct AABB {
    glm::vec2 min;
    glm::vec2 max;
    AABB(const glm::vec4& rect)
        : min(rect.x, rect.y), max(min + glm::vec2(rect.z, rect.w)) {}
};

void updateVelocity(float dt,
                    glm::vec2& velocity,
                    const AABB& a,
                    const AABB& b) {
    const float d1x = b.min.x - a.max.x;
    const float d1y = b.min.y - a.max.y;
    const float d2x = a.min.x - b.max.x;
    const float d2y = a.min.y - b.max.y;

    // check direction
    // x axis
    if (velocity.x > 0 && d1x >= 0) {  // right
        velocity.x = d1x / dt;
    } else if (velocity.x < 0 && d2x >= 0) {  // left
        velocity.x = -d2x / dt;
    }
    // y axis
    if (velocity.y > 0 && d1y >= 0) {  // down
        velocity.y = d1y / dt;
    } else if (velocity.y < 0 && d2y >= 0) {  // up
        velocity.y = -d2y / dt;
    }
}
}  // namespace

CollisionSystem::CollisionSystem(
    std::shared_ptr<inari::EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void CollisionSystem::update(float dt, const inari::EntityPtr& entity) {
    const auto* collision = getRegistry()->getComponent<Collision>(entity);
    if (collision == nullptr || !collision->isDynamic) {
        return;
    }

    if (!getRegistry()->hasComponent<inari::RigidBody>(entity)) {
        return;
    }

    const auto* transform =
        getRegistry()->getComponent<inari::Transform>(entity);
    auto* rigidBody = getRegistry()->getComponent<inari::RigidBody>(entity);
    if (transform == nullptr || rigidBody == nullptr) {
        return;
    }

    if (rigidBody->velocity == glm::vec2(0)) {
        return;
    }

    const glm::vec4 futureRect(
        transform->getAbsolutePosition() + (rigidBody->velocity * dt),
        transform->size);
    const auto callback = [this, entity, aRect = futureRect](
                              const inari::EntityPtr& compareWith) {
        if (entity == compareWith) {
            return false;
        }

        if (!getRegistry()->hasComponent<Collision>(compareWith)) {
            return false;
        }

        const auto* transform =
            getRegistry()->getComponent<inari::Transform>(compareWith);
        if (transform == nullptr) {
            return false;
        }

        const AABB a(aRect);
        const AABB b(transform->getRect());
        const float d1x = b.min.x - a.max.x;
        const float d1y = b.min.y - a.max.y;
        const float d2x = a.min.x - b.max.x;
        const float d2y = a.min.y - b.max.y;

        return d1x < 0.0f && d1y < 0.0f && d2x < 0.0f && d2y < 0.0f;
    };
    const inari::EntityPtr collidedEntity = getRegistry()->findEntity(callback);
    if (collidedEntity == nullptr) {
        return;
    }

    const auto* collidedTransform =
        getRegistry()->getComponent<inari::Transform>(collidedEntity);
    if (collidedTransform == nullptr) {
        return;
    }

    updateVelocity(dt, rigidBody->velocity, transform->getRect(),
                   collidedTransform->getRect());
}
