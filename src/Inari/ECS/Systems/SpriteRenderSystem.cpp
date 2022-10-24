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

void SpriteRenderSystem::draw(float dt) {
    draw(dt, glm::mat4(1.0f), SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(float dt, const glm::mat4& transform) {
    draw(dt, transform, SpriteSortMode::DEFERRED);
}

void SpriteRenderSystem::draw(float dt,
                              const glm::mat4& transform,
                              SpriteSortMode sortMode) {
    auto spriteBatch = m_spriteBatchPtr.lock();
    assert(spriteBatch != nullptr && "Sprite batch is empty");

    spriteBatch->begin(transform, sortMode);
    ISystem::updateSystem(dt);
    spriteBatch->end();
}

void SpriteRenderSystem::update(float dt, const EntityPtr& entity) {
    auto spriteBatch = m_spriteBatchPtr.lock();
    if (spriteBatch == nullptr) {
        return;
    }

    const auto* sprite = m_registry->getComponent<Sprite>(entity);
    const auto* transformation = m_registry->getComponent<Transform>(entity);
    if (sprite == nullptr || transformation == nullptr) {
        return;
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
}  // namespace inari