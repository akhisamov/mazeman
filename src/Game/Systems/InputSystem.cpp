#include "InputSystem.h"

#include <cmath>

#include "Inari/ECS/Components/RigidBody.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"
#include "Inari/InputManager.h"

#include "Game/Components/Player.h"

InputSystem::InputSystem(const std::shared_ptr<inari::InputManager>& inputManager)
    : m_inputPtr(inputManager)
{
}

void InputSystem::update(const inari::GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity)
{
    assert(entityRegistry != nullptr && "Entity Registry is empty");
    auto inputManager = m_inputPtr.lock();
    if (inputManager == nullptr) {
        return;
    }

    auto* transform = entityRegistry->getComponent<inari::Transform>(entity);
    auto* rigidBody = entityRegistry->getComponent<inari::RigidBody>(entity);
    if (rigidBody == nullptr || transform == nullptr || !entityRegistry->hasComponent<Player>(entity)) {
        return;
    }

    rigidBody->velocity = glm::vec2(0.0f);

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

    if (rigidBody->velocity != glm::vec2(0.0f)) {
        transform->radian = std::atan2(-rigidBody->velocity.y, -rigidBody->velocity.x);
    }
}
