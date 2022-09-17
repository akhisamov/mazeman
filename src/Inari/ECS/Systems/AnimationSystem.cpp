#include "AnimationSystem.hpp"

#include <SDL_timer.h>

#include <cmath>
#include <utility>

#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/Sprite.hpp"

namespace inari {
AnimationSystem::AnimationSystem(std::shared_ptr<EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void AnimationSystem::update(float dt) {
    for (const auto& entity : m_registry->getEntities()) {
        if (entity == nullptr) {
            continue;
        }

        auto* sprite = m_registry->getComponent<Sprite>(entity);
        auto* animSprite = m_registry->getComponent<AnimationSprite>(entity);
        if (sprite == nullptr || animSprite == nullptr) {
            continue;
        }

        auto it = animSprite->tracks.find(animSprite->currentTrack);
        if (it != animSprite->tracks.end()) {
            int framesToUpdate = 1;
            if (animSprite->isFramesLimited) {
                const float deltaTime = dt - animSprite->lastUpdate;
                framesToUpdate = static_cast<int>(
                    std::floor(deltaTime / (1.0f / animSprite->framesLimit)));
            }

            if (framesToUpdate > 0) {
                animSprite->currentFrame += framesToUpdate;
                if (animSprite->currentFrame >= it->second.size()) {
                    animSprite->currentFrame = 0;
                }
                animSprite->lastUpdate = dt;
            }

            sprite->sourceRect = it->second[animSprite->currentFrame];
        }
    }
}
}  // namespace inari