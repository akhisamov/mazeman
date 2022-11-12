#include "SpriteRenderSystem.hpp"

#include <cassert>
#include <utility>

#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"

namespace inari {
SpriteRenderSystem::SpriteRenderSystem(
    std::shared_ptr<EntityRegistry> registry,
    const std::shared_ptr<inari::SpriteBatch>& spriteBatch)
    : ISystem(std::move(registry)), m_spriteBatchPtr(spriteBatch) {}

void SpriteRenderSystem::draw(const inari::GameTime& gameTime) {
    draw(gameTime, glm::mat4(1.0f), SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(const inari::GameTime& gameTime,
                              const glm::mat4& transform) {
    draw(gameTime, transform, SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(const inari::GameTime& gameTime,
                              const glm::mat4& transform,
                              SpriteSortMode sortMode) {
    auto spriteBatch = m_spriteBatchPtr.lock();
    assert(spriteBatch != nullptr && "Sprite batch is empty");

    spriteBatch->begin(transform, sortMode);
    ISystem::updateSystem(gameTime);
    spriteBatch->end();
}

void SpriteRenderSystem::update(const inari::GameTime& gameTime,
                                const EntityPtr& entity) {
    auto spriteBatch = m_spriteBatchPtr.lock();
    if (spriteBatch == nullptr) {
        return;
    }

    const auto* sprite = getRegistry()->getComponent<Sprite>(entity);
    const auto* transform = getRegistry()->getComponent<Transform>(entity);
    if (sprite == nullptr || transform == nullptr) {
        return;
    }

    if (transform->size != glm::vec2(0)) {
        spriteBatch->draw(sprite->texture, sprite->color,
                          glm::vec4(transform->position, transform->size),
                          sprite->sourceRect, transform->radian,
                          transform->origin);
    } else {
        spriteBatch->draw(sprite->texture, sprite->color, transform->position,
                          sprite->sourceRect, transform->radian,
                          transform->origin);
    }
}
}  // namespace inari
