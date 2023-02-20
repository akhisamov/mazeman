#pragma once

#include <glm/mat4x4.hpp>

#include "ISystem.h"

namespace inari {
    class SpriteBatch;
    enum class SpriteSortMode;

    class SpriteRenderSystem : public ISystem {
    public:
        void begin();
        void begin(const glm::mat4& transform);
        void begin(const glm::mat4& transform, SpriteSortMode sortMode);
        void end();

    protected:
        void update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity) override;
    };
} // namespace inari
