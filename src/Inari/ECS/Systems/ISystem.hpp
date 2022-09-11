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
        virtual ~ISystem() = default;

    protected:
        std::shared_ptr<EntityRegistry> m_registry;
    };
}