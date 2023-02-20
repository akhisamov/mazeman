#include "AnimationSystem.h"

#include <cmath>

#include "Inari/ECS/Components/AnimationSprite.h"
#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/EntityRegistry.h"

#include "Inari/Utils/GameTime.h"

namespace inari {
    void AnimationSystem::update(const GameTime& gameTime, const EntityRegPtr& entityRegistry, const EntityPtr& entity)
    {
        assert(entityRegistry != nullptr && "Entity Registry is empty");

        auto* sprite = entityRegistry->getComponent<Sprite>(entity);
        auto* animSprite = entityRegistry->getComponent<AnimationSprite>(entity);
        if (sprite == nullptr || animSprite == nullptr) {
            return;
        }

        const auto currentTicks = static_cast<float>(GameTime::getCurrentTicks());

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
