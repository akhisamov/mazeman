#pragma once

#include <memory>
#include <utility>

#include "Inari/ECS/EntityRegistry.hpp"

namespace inari
{
    class ISystem
    {
    public:
        explicit ISystem(std::shared_ptr<EntityRegistry> registry)
            : m_registry(std::move(registry))
        {
        }
        virtual ~ISystem() = default;

    protected:
        std::shared_ptr<EntityRegistry> m_registry;
    };
}