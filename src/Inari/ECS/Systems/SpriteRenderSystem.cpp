#include "SpriteRenderSystem.h"

#include <cassert>
#include <utility>

#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"

#include "Inari/Graphics/SpriteBatch.hpp"

namespace inari {
    SpriteRenderSystem::SpriteRenderSystem(const std::shared_ptr<SpriteBatch>& spriteBatch)
        : m_spriteBatchPtr(spriteBatch)
    {
    }

    void SpriteRenderSystem::begin() { begin(glm::mat4(1.0f), SpriteSortMode::DEFERRED); }

    void SpriteRenderSystem::begin(const glm::mat4& transform) { begin(transform, SpriteSortMode::DEFERRED); }

    void SpriteRenderSystem::begin(const glm::mat4& transform, SpriteSortMode sortMode)
    {
        auto spriteBatch = m_spriteBatchPtr.lock();
        assert(spriteBatch != nullptr && "Sprite batch is empty");

        spriteBatch->begin(transform, sortMode);
    }

    void SpriteRenderSystem::end()
    {
        auto spriteBatch = m_spriteBatchPtr.lock();
        assert(spriteBatch != nullptr && "Sprite batch is empty");
        spriteBatch->end();
    }

    void SpriteRenderSystem::update(const GameTime& gameTime, const EntityRegPtr& entityRegistry,
                                    const EntityPtr& entity)
    {
        assert(entityRegistry != nullptr && "Entity registry is empty");

        auto spriteBatch = m_spriteBatchPtr.lock();
        if (spriteBatch == nullptr) {
            return;
        }

        const auto* sprite = entityRegistry->getComponent<Sprite>(entity);
        const auto* transform = entityRegistry->getComponent<Transform>(entity);
        if (sprite == nullptr || transform == nullptr) {
            return;
        }

        if (transform->size != glm::vec2(0)) {
            spriteBatch->draw(sprite->texture, sprite->color, glm::vec4(transform->position, transform->size),
                              sprite->sourceRect, transform->radian, transform->origin);
        } else {
            spriteBatch->draw(sprite->texture, sprite->color, transform->position, sprite->sourceRect,
                              transform->radian, transform->origin);
        }
    }
} // namespace inari
