#include "InputSystem.hpp"

#include <cmath>

#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/InputManager.hpp"

#include "Game/Components/Player.hpp"

InputSystem::InputSystem(
    std::shared_ptr<inari::EntityRegistry> registry,
    const std::shared_ptr<inari::InputManager>& inputManager)
    : ISystem(std::move(registry)), m_inputPtr(inputManager) {}

void InputSystem::update(const inari::GameTime& gameTime,
                         const inari::EntityPtr& entity) {
    auto inputManager = m_inputPtr.lock();
    if (inputManager == nullptr) {
        return;
    }

    auto* transform = getRegistry()->getComponent<inari::Transform>(entity);
    auto* rigidBody = getRegistry()->getComponent<inari::RigidBody>(entity);
    if (rigidBody == nullptr || transform == nullptr ||
        !getRegistry()->hasComponent<Player>(entity)) {
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
        transform->radian =
            std::atan2(-rigidBody->velocity.y, -rigidBody->velocity.x);
    }
}
