#include "AnimationSystem.hpp"

#include <cmath>

#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/Sprite.hpp"

#include "Inari/Utils/GameTime.hpp"

namespace inari {
    AnimationSystem::AnimationSystem(std::shared_ptr<EntityRegistry> registry)
        : ISystem(std::move(registry))
    {
    }

    void AnimationSystem::update(const inari::GameTime& gameTime, const EntityPtr& entity)
    {
        auto* sprite = getRegistry()->getComponent<Sprite>(entity);
        auto* animSprite = getRegistry()->getComponent<AnimationSprite>(entity);
        if (sprite == nullptr || animSprite == nullptr) {
            return;
        }

        const auto currentTicks = static_cast<float>(inari::GameTime::getCurrentTicks());

        auto it = animSprite->tracks.find(animSprite->currentTrack);
        if (it != animSprite->tracks.end()) {
            int framesToUpdate = 1;
            if (animSprite->isFramesLimited) {
                const float deltaTime = (currentTicks - animSprite->lastUpdate) / 1000.0f;
                framesToUpdate = static_cast<int>(std::floor(deltaTime / (1.0f / animSprite->framesLimit)));
            }

            if (framesToUpdate > 0) {
                animSprite->currentFrame += framesToUpdate;
                if (animSprite->currentFrame >= it->second.size()) {
                    animSprite->currentFrame = 0;
                }
                animSprite->lastUpdate = currentTicks;
            }

            sprite->sourceRect = it->second[animSprite->currentFrame];
        }
    }
} // namespace inari
