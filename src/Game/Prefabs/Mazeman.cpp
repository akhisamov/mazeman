#include "Mazeman.h"

#include <algorithm>

#include <glm/trigonometric.hpp>

#include "Inari/ECS/Components/AnimationSprite.h"
#include "Inari/ECS/Components/RigidBody.h"
#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"

#include "Game/Components/Collision.h"
#include "Game/Components/Player.h"

namespace {
    constexpr glm::vec2 size(32);
}

namespace prefabs {
    struct TracksGenerator {
        glm::vec4 operator()() { return { static_cast<float>(i++) * size.x, 0, size }; }

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
                       const std::shared_ptr<inari::Texture2D>& texture, const glm::vec2& position, float angle)
    {
        constexpr std::string_view name = "mazeman";
        const auto mazeman = entityRegistry->createEntity(name);

        if (texture) {
            entityRegistry->emplaceComponent(mazeman, inari::Sprite { texture });
        }

        {
            inari::Transform transform;
            transform.origin = glm::vec2(0.5f, 0.5f);
            transform.position = position;
            transform.size = size;
            transform.radian = glm::radians(angle);
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