#pragma once

#include "Inari/ECS/Systems/ISystem.h"

namespace inari {
    class InputManager;
}

class InputSystem : public inari::ISystem {
protected:
    void update(const inari::GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;
};
