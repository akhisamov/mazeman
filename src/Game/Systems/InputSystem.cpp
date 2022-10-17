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

void InputSystem::update(float dt) {
    auto inputManager = m_inputPtr.lock();
    if (inputManager == nullptr || m_registry == nullptr) {
        return;
    }

    for (const auto& entity : m_registry->getEntities()) {
        if (entity == nullptr) {
            continue;
        }

        auto* transform = m_registry->getComponent<inari::Transform>(entity);
        auto* rigidBody = m_registry->getComponent<inari::RigidBody>(entity);
        if (rigidBody == nullptr || transform == nullptr ||
            !m_registry->hasComponent<Player>(entity)) {
            continue;
        }

        rigidBody->velocity = glm::vec2(0.0f);

        constexpr float playerSpeed = 1.0f;
        if (inputManager->isKeyDown(SDLK_UP)) {
            rigidBody->velocity.y = -playerSpeed;
        }
        if (inputManager->isKeyDown(SDLK_DOWN)) {
            rigidBody->velocity.y = playerSpeed;
        }
        if (inputManager->isKeyDown(SDLK_RIGHT)) {
            rigidBody->velocity.x = playerSpeed;
        }
        if (inputManager->isKeyDown(SDLK_LEFT)) {
            rigidBody->velocity.x = -playerSpeed;
        }

        if (rigidBody->velocity != glm::vec2(0.0f)) {
            transform->radian =
                std::atan2(-rigidBody->velocity.y, -rigidBody->velocity.x);
        }
    }
}