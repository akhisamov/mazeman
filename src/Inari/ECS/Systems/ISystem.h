#pragma once

#include <memory>

namespace inari {
    class EntityRegistry;
    struct Entity;
    class GameTime;

    class ISystem {
    protected:
        using EntityRegPtr = std::shared_ptr<EntityRegistry>;
        using EntityPtr = std::shared_ptr<Entity>;

    public:
        virtual ~ISystem() = default;
        virtual void update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) = 0;
    };
} // namespace inari
