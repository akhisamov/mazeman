#include "PhysicsSystem.h"

#include "Inari/ECS/Components/RigidBody.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"
#include "Inari/ECS/Systems/ISystem.h"

#include "Inari/Utils/GameTime.h"

namespace inari {
    void PhysicsSystem::update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity)
    {
        assert(entityRegistry != nullptr && "EntityRegistry is empty");
        auto* transform = entityRegistry->getComponent<Transform>(entity);
        auto* rigidBody = entityRegistry->getComponent<RigidBody>(entity);
        if (transform == nullptr || rigidBody == nullptr) {
            return;
        }

        transform->position += rigidBody->velocity * gameTime.getElapsedTime();
    }

} // namespace inari
