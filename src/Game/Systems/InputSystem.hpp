#pragma once

#include "Inari/ECS/Systems/ISystem.hpp"

namespace inari {
class InputManager;
}

class InputSystem : public inari::ISystem {
   public:
    explicit InputSystem(
        std::shared_ptr<inari::EntityRegistry> registry,
        const std::shared_ptr<inari::InputManager>& inputManager);

   protected:
    void update(float dt, const inari::EntityPtr& entity) override;

   private:
    std::weak_ptr<inari::InputManager> m_inputPtr;
};