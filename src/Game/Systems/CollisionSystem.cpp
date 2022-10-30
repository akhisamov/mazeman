#include "CollisionSystem.hpp"

#include <glm/geometric.hpp>

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Game/Components/Collision.hpp"

namespace {
glm::vec2 getAbsolutePosition(const inari::Transform* transform) {
    return transform->position - (transform->size * transform->origin);
}

glm::vec4 getRect(const inari::Transform* transform) {
    return {getAbsolutePosition(transform), transform->size};
}
}  // namespace

CollisionSystem::CollisionSystem(
    std::shared_ptr<inari::EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

// void CollisionSystem::update(float dt, const inari::EntityPtr& entity) {
//     const auto* collision = getRegistry()->getComponent<Collision>(entity);
//     if (collision == nullptr || !collision->isDynamic) {
//         return;
//     }

//     const auto* transform =
//         getRegistry()->getComponent<inari::Transform>(entity);
//     auto* rigidBody = getRegistry()->getComponent<inari::RigidBody>(entity);
//     if (transform == nullptr || rigidBody == nullptr) {
//         return;
//     }

//     const glm::vec4 oldRect = getRect(transform);
//     const glm::vec4 newRect(
//         getAbsolutePosition(transform) + (rigidBody->velocity * dt),
//         transform->size);

//     const auto callback = [this, entity, oldRect, newRect,
//                            rigidBody](const inari::EntityPtr& otherEntity) {
//         if (entity == otherEntity) {
//             return false;
//         }

//         if (!getRegistry()->hasComponent<Collision>(otherEntity)) {
//             return false;
//         }

//         const auto* transform =
//             getRegistry()->getComponent<inari::Transform>(otherEntity);
//         if (transform == nullptr) {
//             return false;
//         }

//         const glm::vec4 otherRect = getRect(transform);

//         const bool leftCollision = oldRect.x + oldRect.z < otherRect.x &&
//                                    newRect.x + newRect.z >= otherRect.x;
//         const bool rightCollision = oldRect.x >= otherRect.x + otherRect.z &&
//                                     newRect.x < otherRect.x + otherRect.z;
//         const bool topCollision = oldRect.y + oldRect.w < otherRect.y &&
//                                   newRect.y + newRect.w >= otherRect.y;
//         const bool bottomCollision = oldRect.y >= otherRect.y + otherRect.w
//         &&
//                                      newRect.y < otherRect.y + otherRect.w;

//         if (topCollision || bottomCollision) {
//             rigidBody->velocity.y = 0;
//         }
//         if (leftCollision || rightCollision) {
//             rigidBody->velocity.x = 0;
//         }

//         return topCollision || bottomCollision || leftCollision ||
//                rightCollision;
//     };

//     getRegistry()->anyOfEntity(callback);
// }

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

    const glm::vec4 oldRect = getRect(transform);
    const glm::vec4 newRect(
        getAbsolutePosition(transform) + (rigidBody->velocity * dt),
        transform->size);

    // const bool isCollided =
    //     getRegistry()->findEntity(
    //         [this, entity](const inari::EntityPtr& compareWith) {
    //             return check(entity, compareWith);
    //         }) != nullptr;

    const bool isCollided =
        getRegistry()->findEntity(
            [this, entity, newRect](const inari::EntityPtr& compareWith) {
                if (entity == compareWith) {
                    return false;
                }
                return check(newRect, compareWith);
            }) != nullptr;

    if (isCollided) {
        rigidBody->velocity = glm::vec2(0);
    }
}

bool CollisionSystem::check(const inari::EntityPtr& a,
                            const inari::EntityPtr& b) const {
    if (a == b) {
        return false;
    }

    if (!getRegistry()->hasComponent<Collision>(a) ||
        !getRegistry()->hasComponent<Collision>(b)) {
        return false;
    }

    const auto* aTransform = getRegistry()->getComponent<inari::Transform>(a);
    const auto* bTransform = getRegistry()->getComponent<inari::Transform>(b);
    if (aTransform == nullptr || bTransform == nullptr) {
        return false;
    }

    const glm::vec4 aRect = getRect(aTransform);
    const glm::vec4 bRect = getRect(bTransform);

    return aRect.x < bRect.x + bRect.z &&  // left
           aRect.x + aRect.z > bRect.x &&  // right
           aRect.y < bRect.y + bRect.w &&  // top
           aRect.y + aRect.w > bRect.y;    // bottom
}

bool CollisionSystem::check(const glm::vec4& aRect,
                            const inari::EntityPtr& b) const {
    if (!getRegistry()->hasComponent<Collision>(b)) {
        return false;
    }

    const auto* bTransform = getRegistry()->getComponent<inari::Transform>(b);
    if (bTransform == nullptr) {
        return false;
    }

    const glm::vec4 bRect = getRect(bTransform);

    bool result = aRect.x < bRect.x + bRect.z &&  // left
                  aRect.x + aRect.z > bRect.x &&  // right
                  aRect.y < bRect.y + bRect.w &&  // top
                  aRect.w + aRect.y > bRect.y;    // bottom
    if (result) {
        int a = 1;
    }
    return result;
}