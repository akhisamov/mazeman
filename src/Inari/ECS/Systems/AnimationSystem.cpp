#include "AnimationSystem.hpp"

#include "Inari/ECS/Components/AnimationSprite.hpp"

namespace inari
{
    AnimationSystem::AnimationSystem(const std::shared_ptr<EntityRegistry>& registry)
        : ISystem(registry)
    {
    }

    void AnimationSystem::update(float dt)
    {
        for (const auto& entity : m_registry->getEntities())
        {
            if (entity == nullptr)
            {
                continue;
            }

            auto* animSprite = m_registry->getComponent<AnimationSprite>(entity);
            if (animSprite == nullptr)
            {
                continue;
            }

            auto it = animSprite->tracks.find(animSprite->currentTrack);
            if (it != animSprite->tracks.end())
            {
                // todo set fps limit
                //int framesToUpdate = static_cast<int>(floor(dt / (1.0f / 24)));
                //if (framesToUpdate > 0)
                //{
                //    animSprite->currentFrame += framesToUpdate;
                //    if (animSprite->currentFrame >= it->second.size())
                //    {
                //        animSprite->currentFrame = 0;
                //    }
                //}

                animSprite->currentFrame++;
                if (animSprite->currentFrame >= it->second.size())
                {
                    animSprite->currentFrame = 0;
                }

                animSprite->sourceRect = it->second[animSprite->currentFrame];
            }
        }
    }
}