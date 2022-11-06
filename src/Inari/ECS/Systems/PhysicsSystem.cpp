#include "PhysicsSystem.hpp"

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/Systems/ISystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"

namespace inari {

PhysicsSystem::PhysicsSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void PhysicsSystem::update(float dt, const EntityPtr& entity) {
    auto* transform = getRegistry()->getComponent<Transform>(entity);
    auto* rigidBody = getRegistry()->getComponent<RigidBody>(entity);
    if (transform == nullptr || rigidBody == nullptr) {
        return;
    }

    transform->position += rigidBody->velocity * dt;
}

}  // namespace inari