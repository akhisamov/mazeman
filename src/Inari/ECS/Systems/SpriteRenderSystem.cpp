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
                              const std::shared_ptr<SpriteBatch>& spriteBatch,
                              const glm::mat4& transform) {
    assert(spriteBatch != nullptr && "Sprite batch is empty");

    spriteBatch->begin(transform);
    for (const auto& entity : m_registry->getEntities()) {
        if (entity == nullptr) {
            continue;
        }

        const auto* sprite = m_registry->getComponent<Sprite>(entity);
        const auto* transform = m_registry->getComponent<Transform>(entity);
        if (sprite == nullptr || transform == nullptr) {
            continue;
        }

        if (sprite->size != glm::vec2(0)) {
            const glm::vec4 destRect(transform->position, sprite->size);
            spriteBatch->draw(sprite->texture, sprite->color, destRect,
                              sprite->sourceRect, transform->radian,
                              transform->origin);
        } else {
            spriteBatch->draw(sprite->texture, sprite->color,
                              transform->position, sprite->sourceRect,
                              transform->radian, transform->origin);
        }
    }
    spriteBatch->end();
}
}  // namespace inari