#include "Pacman.hpp"

#include <algorithm>

#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/EntityRegistry.hpp"

#include "Game/Components/Player.hpp"

namespace {
struct TracksGenerator {
    glm::vec4 operator()() {
        auto result = glm::vec4(0, 0, 32, 32);
        result.x = static_cast<float>(i) * 32.0f;
        result.z = static_cast<float>(++i) * 32.0f;
        return result;
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
}  // namespace

namespace prefabs {
void createPacman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                  const std::shared_ptr<inari::Texture2D>& texture) {
    constexpr std::string_view name = "pacman";
    const auto pacman = entityRegistry->createEntity(name);

    if (texture) {
        entityRegistry->emplaceComponent(pacman,
                                         inari::Sprite{texture, glm::vec2(32)});
    }

    entityRegistry->emplaceComponent<inari::Transform>(pacman);
    entityRegistry->emplaceComponent(pacman, createAnimationSprite());
    entityRegistry->emplaceComponent<inari::RigidBody>(pacman);
    entityRegistry->emplaceComponent<Player>(pacman);
}
}  // namespace prefabs