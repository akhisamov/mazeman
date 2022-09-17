#pragma once

#include <glm/mat4x4.hpp>

#include "ISystem.hpp"

namespace inari {
class SpriteBatch;
enum class SpriteSortMode;

class SpriteRenderSystem : public ISystem {
   public:
    explicit SpriteRenderSystem(std::shared_ptr<EntityRegistry> registry);

    void draw(float dt, const std::shared_ptr<SpriteBatch>& spriteBatch);
    void draw(float dt,
              const std::shared_ptr<SpriteBatch>& spriteBatch,
              const glm::mat4& transform);
    void draw(float dt,
              const std::shared_ptr<SpriteBatch>& spriteBatch,
              const glm::mat4& transform,
              SpriteSortMode sortMode);
};
}  // namespace inari