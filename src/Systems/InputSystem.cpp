#include "InputSystem.h"

#include <cmath>

#include "Inari/ECS/Components/RigidBody.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"
#include "Inari/GameServices.h"
#include "Inari/InputManager.h"

#include "Components/Player.h"

void InputSystem::update(const inari::GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity)
{
    assert(entityRegistry != nullptr && "Entity Registry is empty");

    const auto& inputManager = inari::GameServices::get<inari::InputManager>();
    if (inputManager == nullptr) {
        return;
    }

    auto* transform = entityRegistry->getComponent<inari::Transform>(entity);
    auto* rigidBody = entityRegistry->getComponent<inari::RigidBody>(entity);
    if (rigidBody == nullptr || transform == nullptr || !entityRegistry->hasComponent<Player>(entity)) {
        return;
    }

    rigidBody->velocity = inari::Vec2f::Zero();

    if (inputManager->isKeyDown(SDLK_UP)) {
        rigidBody->velocity.y = -rigidBody->speed;
    }
    if (inputManager->isKeyDown(SDLK_DOWN)) {
        rigidBody->velocity.y = rigidBody->speed;
    }
    if (inputManager->isKeyDown(SDLK_RIGHT)) {
        rigidBody->velocity.x = rigidBody->speed;
    }
    if (inputManager->isKeyDown(SDLK_LEFT)) {
        rigidBody->velocity.x = -rigidBody->speed;
    }

    if (rigidBody->velocity.x != 0 || rigidBody->velocity.y != 0) {
        transform->radian = std::atan2(-rigidBody->velocity.y, -rigidBody->velocity.x);
    }
}
