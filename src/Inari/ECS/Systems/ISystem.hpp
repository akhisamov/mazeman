#pragma once

#include <memory>

#include "Inari/ECS/EntityRegistry.hpp"

namespace inari
{
    class GameTime;

    class ISystem
    {
    public:
        ISystem(const std::shared_ptr<inari::EntityRegistry>& registry)
            : m_registry(registry)
        {
        }

        virtual void update(const inari::GameTime& gameTime) = 0;

    private:
        std::shared_ptr<inari::EntityRegistry> m_registry;
    };
}