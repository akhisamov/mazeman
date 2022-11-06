#include "Mazeman.hpp"

#include <algorithm>
#include <cmath>

#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/EntityRegistry.hpp"

#include "Inari/Utils/Math.hpp"

#include "Game/Components/Collision.hpp"
#include "Game/Components/Player.hpp"

namespace {
constexpr glm::vec2 size(32);
}

namespace prefabs {
struct TracksGenerator {
    glm::vec4 operator()() {
        return {static_cast<float>(i++) * size.x, 0, size};
    }

   private:
    int i = 0;
};

inari::AnimationSprite createAnimationSprite() {
    inari::AnimationSprite animSprite;
    animSprite.currentTrack = "default";
    animSprite.isFramesLimited = true;
    animSprite.framesLimit = 24.0f;
    auto& defaultTracks = animSprite.tracks[animSprite.currentTrack];
    defaultTracks.resize(6);
    std::generate(defaultTracks.begin(), defaultTracks.end(),
                  TracksGenerator());
    return animSprite;
}

void createMazeman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                   const std::shared_ptr<inari::Texture2D>& texture,
                   const glm::vec2& position,
                   float angle) {
    constexpr std::string_view name = "mazeman";
    const auto mazeman = entityRegistry->createEntity(name);

    if (texture) {
        entityRegistry->emplaceComponent(mazeman, inari::Sprite{texture});
    }

    {
        inari::Transform transform;
        transform.origin = glm::vec2(0.5f, 0.5f);
        transform.position = position;
        transform.size = size;
        transform.radian = inari::math::degreesToRadians(angle);
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
}  // namespace prefabs