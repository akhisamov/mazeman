#include "SpriteRenderSystem.hpp"

#include <cassert>
#include <utility>

#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"

namespace inari {
SpriteRenderSystem::SpriteRenderSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void SpriteRenderSystem::draw(float dt,
                              const std::shared_ptr<SpriteBatch>& spriteBatch) {
    draw(dt, spriteBatch, glm::mat4(1.0f), SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(float dt,
                              const std::shared_ptr<SpriteBatch>& spriteBatch,
                              const glm::mat4& transform) {
    draw(dt, spriteBatch, transform, SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(float dt,
                              const std::shared_ptr<SpriteBatch>& spriteBatch,
                              const glm::mat4& transform,
                              SpriteSortMode sortMode) {
    assert(spriteBatch != nullptr && "Sprite batch is empty");

    spriteBatch->begin(transform, sortMode);
    for (const auto& entity : m_registry->getEntities()) {
        if (entity == nullptr) {
            continue;
        }

        const auto* sprite = m_registry->getComponent<Sprite>(entity);
        const auto* transformation =
            m_registry->getComponent<Transform>(entity);
        if (sprite == nullptr || transformation == nullptr) {
            continue;
        }

        if (sprite->size != glm::vec2(0)) {
            const glm::vec4 destRect(transformation->position, sprite->size);
            spriteBatch->draw(sprite->texture, sprite->color, destRect,
                              sprite->sourceRect, transformation->radian,
                              transformation->origin);
        } else {
            spriteBatch->draw(sprite->texture, sprite->color,
                              transformation->position, sprite->sourceRect,
                              transformation->radian, transformation->origin);
        }
    }
    spriteBatch->end();
}
}  // namespace inari