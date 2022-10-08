#include "Pacman.hpp"

#include <algorithm>
#include <cmath>

#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/EntityRegistry.hpp"

#include "Game/Components/Player.hpp"

namespace prefabs {
struct TracksGenerator {
    glm::vec4 operator()() {
        return {static_cast<float>(i++) * 32.0f, 0, 32, 32};
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

void createPacman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                  const std::shared_ptr<inari::Texture2D>& texture) {
    constexpr std::string_view name = "pacman";
    const auto pacman = entityRegistry->createEntity(name);

    if (texture) {
        entityRegistry->emplaceComponent(pacman,
                                         inari::Sprite{texture, glm::vec2(32)});
    }

    {
        inari::Transform transform;
        transform.origin = glm::vec2(0.5f, 0.5f);
        transform.position = glm::vec2(200, 200);
        transform.radian = std::atan2(-1, 0);
        entityRegistry->emplaceComponent(pacman, transform);
    }
    entityRegistry->emplaceComponent(pacman, createAnimationSprite());
    entityRegistry->emplaceComponent<inari::RigidBody>(pacman);
    entityRegistry->emplaceComponent<Player>(pacman);
}
}  // namespace prefabs