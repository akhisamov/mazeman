#pragma once

#include "Inari/ECS/Systems/ISystem.h"

namespace inari {
    class InputManager;
}

class InputSystem : public inari::ISystem {
public:
    explicit InputSystem(const std::shared_ptr<inari::InputManager>& inputManager);

protected:
    void update(const inari::GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;

private:
    std::weak_ptr<inari::InputManager> m_inputPtr;
};
