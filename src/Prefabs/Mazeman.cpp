#include "Mazeman.h"

#include <algorithm>

#include <Inari/Common/Math.h>

#include "Inari/ECS/Components/AnimationSprite.h"
#include "Inari/ECS/Components/RigidBody.h"
#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"

#include "Components/Collision.h"
#include "Components/Player.h"

namespace {
    constexpr inari::Vec2f size(32);
}

namespace prefabs {
    struct TracksGenerator {
        inari::Vec4f operator()() { return { static_cast<float>(i++) * size.x, 0, size }; }

    private:
        int i = 0;
    };

    inari::AnimationSprite createAnimationSprite()
    {
        inari::AnimationSprite animSprite;
        animSprite.currentTrack = "default";
        animSprite.isFramesLimited = true;
        animSprite.framesLimit = 24.0f;
        auto& defaultTracks = animSprite.tracks[animSprite.currentTrack];
        defaultTracks.resize(6);
        std::generate(defaultTracks.begin(), defaultTracks.end(), TracksGenerator());
        return animSprite;
    }

    void createMazeman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                       const std::shared_ptr<inari::Texture2D>& texture, const inari::Vec2f& position, float angle)
    {
        constexpr std::string_view name = "mazeman";
        const auto mazeman = entityRegistry->createEntity(name);

        if (texture) {
            entityRegistry->emplaceComponent(mazeman, inari::Sprite { texture });
        }

        {
            inari::Transform transform;
            transform.origin = inari::Vec2f(0.5f, 0.5f);
            transform.position = position;
            transform.size = size;
            transform.radian = inari::math::radians(angle);
            entityRegistry->emplaceComponent(mazeman, transform);
        }
        {
            inari::RigidBody rigidBody;
            rigidBody.speed = 100.0f;
            entityRegistry->emplaceComponent(mazeman, rigidBody);
        }
        entityRegistry->emplaceComponent(mazeman, createAnimationSprite());
        entityRegistry->emplaceComponent<Player>(mazeman);
        entityRegistry->emplaceComponent<Collision>(mazeman);
    }
} // namespace prefabs