#pragma once

#include <memory>

#include "Inari/ECS/EntityRegistry.hpp"

namespace inari
{
    class ISystem
    {
    public:
        ISystem(const std::shared_ptr<EntityRegistry>& registry)
            : m_registry(registry)
        {
        }

        virtual void update(float dt) = 0;

    protected:
        std::shared_ptr<EntityRegistry> m_registry;
    };
}