#pragma once

#include <glm/mat4x4.hpp>

#include "ISystem.hpp"

namespace inari {
class SpriteBatch;
enum class SpriteSortMode;

class SpriteRenderSystem : public ISystem {
   public:
    explicit SpriteRenderSystem(
        std::shared_ptr<EntityRegistry> registry,
        const std::shared_ptr<inari::SpriteBatch>& spriteBatch);

    void updateSystem(float dt) = delete;

    void draw(float dt);
    void draw(float dt, const glm::mat4& transform);
    void draw(float dt, const glm::mat4& transform, SpriteSortMode sortMode);

   protected:
    void update(float dt, const EntityPtr& entity) override;

   private:
    std::weak_ptr<inari::SpriteBatch> m_spriteBatchPtr;
};
}  // namespace inari