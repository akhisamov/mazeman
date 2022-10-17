#include "PhysicsSystem.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/Systems/ISystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"

namespace inari {

PhysicsSystem::PhysicsSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void PhysicsSystem::update(float dt) {
    for (const auto& entity : m_registry->getEntities()) {
        if (entity == nullptr) {
            continue;
        }

        auto* transform = m_registry->getComponent<Transform>(entity);
        auto* rigidBody = m_registry->getComponent<RigidBody>(entity);
        if (transform == nullptr || rigidBody == nullptr) {
            continue;
        }

        transform->position += rigidBody->velocity * dt;
    }
}

}  // namespace inari