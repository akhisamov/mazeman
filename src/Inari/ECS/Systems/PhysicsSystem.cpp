#include "PhysicsSystem.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/Systems/ISystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"

namespace inari {

PhysicsSystem::PhysicsSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void PhysicsSystem::update(float dt, const EntityPtr& entity) {
    auto* transform = m_registry->getComponent<Transform>(entity);
    auto* rigidBody = m_registry->getComponent<RigidBody>(entity);
    if (transform == nullptr || rigidBody == nullptr) {
        return;
    }

    transform->position += rigidBody->velocity * dt;
}

}  // namespace inari