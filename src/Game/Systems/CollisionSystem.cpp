#include "CollisionSystem.hpp"

#include <glm/geometric.hpp>

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Game/Components/Collision.hpp"

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

    const AABB bounds(
        {transform->getAbsolutePosition() + (rigidBody->velocity * dt),
         transform->size});

    const inari::EntityPtr collidedEntity = getRegistry()->findEntity(
        [this, entity, bounds](const inari::EntityPtr& compareWith) {
            if (entity == compareWith) {
                return false;
            }

            if (!getRegistry()->hasComponent<Collision>(compareWith)) {
                return false;
            }

            const auto* bTransform =
                getRegistry()->getComponent<inari::Transform>(compareWith);
            if (bTransform == nullptr) {
                return false;
            }

            const AABB secondBounds(bTransform->getRect());
            return testAABBOverlap(bounds, secondBounds);
        });
    if (collidedEntity == nullptr) {
        return;
    }

    const auto* collidedTransform =
        getRegistry()->getComponent<inari::Transform>(collidedEntity);
    if (collidedTransform == nullptr) {
        return;
    }
    const glm::vec4 collidedRect = collidedTransform->getRect();

    // get minimal distance between points
}

bool CollisionSystem::testAABBOverlap(const AABB& a, const AABB& b) const {
    const float d1x = b.min.x - a.max.x;
    const float d1y = b.min.y - a.max.y;
    const float d2x = a.min.x - b.max.x;
    const float d2y = a.min.y - b.max.y;

    if (d1x > 0.0f || d1y > 0.0f) {
        return false;
    }

    if (d2x > 0.0f || d2y > 0.0f) {
        return false;
    }

    return true;
}
