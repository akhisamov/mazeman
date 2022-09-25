#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <algorithm>

#include <Generated/shaders/sprite.hpp>

// inari
#include "Inari/ECS/Components/AnimationSprite.hpp"
#include "Inari/ECS/Components/RigidBody.hpp"
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/EntityRegistry.hpp"
#include "Inari/ECS/SystemRegistry.hpp"
#include "Inari/ECS/Systems/AnimationSystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"
#include "Inari/ECS/Systems/SpriteRenderSystem.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"

#include "Inari/InputManager.hpp"
#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Resources/Shader.hpp"

#include "Inari/Utils/Camera2D.hpp"
#include "Inari/Utils/Colors.hpp"
#include "Inari/Utils/Math.hpp"
#include "Inari/Utils/Strings.hpp"
// inari

using namespace inari;

namespace Constants {
constexpr std::string_view title = "PacMan";
constexpr int screenFps = 30;
constexpr glm::ivec2 windowSize(1280, 720);
constexpr glm::vec4 bgColor = colors::Black;
}  // namespace Constants

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

Game::Game()
    : m_entityRegistry(std::make_shared<EntityRegistry>()),
      m_systemRegistry(std::make_unique<SystemRegistry>()),
      m_camera(nullptr) {}

Game::~Game() = default;

bool Game::init() {
    if (IGame::init()) {
        // Init window
        const auto& window = getWindow();
        window->setWindowSize(Constants::windowSize);
        window->setTitle(Constants::title);
        window->setFrameLimit(Constants::screenFps);

        // Init resources
        const auto& resources = getResourceManager();
        resources->addSearchPath("sprites.bundle", "sprites");
        resources->addFile<Texture2D>("pacman", "sprites/pacman.png");

        // Init camera
        m_camera = std::make_unique<Camera2D>(Constants::windowSize, 0.5f);

        // Init systems
        m_systemRegistry->addSystem<AnimationSystem>(m_entityRegistry);
        m_systemRegistry->addSystem<SpriteRenderSystem>(m_entityRegistry);
        m_systemRegistry->addSystem<PhysicsSystem>(m_entityRegistry);

        return true;
    }
    return false;
}

void Game::loadResources() {
    const EntityPtr pacman = m_entityRegistry->createEntity("pacman");
    m_entityRegistry->emplaceComponent<Transform>(pacman);

    if (auto texture = getResourceManager()->load<Texture2D>("pacman")) {
        m_entityRegistry->emplaceComponent(pacman,
                                           Sprite{texture, glm::vec2(32)});
    }

    {
        AnimationSprite animSprite;
        animSprite.currentTrack = "default";
        animSprite.isFramesLimited = true;
        animSprite.framesLimit = 24.0f;
        auto& defaultTracks = animSprite.tracks[animSprite.currentTrack];
        defaultTracks.resize(6);
        std::generate(defaultTracks.begin(), defaultTracks.end(),
                      TracksGenerator());
        m_entityRegistry->emplaceComponent(pacman, animSprite);
    }

    m_entityRegistry->emplaceComponent<RigidBody>(pacman);
}

void Game::unloadResources() {
    getResourceManager()->unload<Texture2D>("pacman");
}

void Game::handleWindowResized(const glm::ivec2& size) {
    m_camera->setWindowSize(size);
}

void Game::update(float dt) {
    auto* rigidBody = m_entityRegistry->getComponent<RigidBody>(
        m_entityRegistry->getEntity("pacman"));
    if (rigidBody) {
        rigidBody->velocity = glm::vec2(0.0f);

        constexpr float pacmanSpeed = 1.0f;
        const auto& inputManager = getInputManager();
        if (inputManager->isKeyDown(SDLK_UP)) {
            rigidBody->velocity.y = pacmanSpeed;
        }
        if (inputManager->isKeyDown(SDLK_DOWN)) {
            rigidBody->velocity.y = -pacmanSpeed;
        }
        if (inputManager->isKeyDown(SDLK_RIGHT)) {
            rigidBody->velocity.x = pacmanSpeed;
        }
        if (inputManager->isKeyDown(SDLK_LEFT)) {
            rigidBody->velocity.x = -pacmanSpeed;
        }
    }

    auto physics = m_systemRegistry->getSystem<PhysicsSystem>();
    if (physics) {
        physics->update(dt);
    }

    auto animation = m_systemRegistry->getSystem<AnimationSystem>();
    if (animation) {
        animation->update(dt);
    }
}

void Game::draw(float dt) {
    getWindow()->clear(Constants::bgColor);

    auto spriteRenderSystem = m_systemRegistry->getSystem<SpriteRenderSystem>();
    if (spriteRenderSystem) {
        spriteRenderSystem->draw(dt, getSpriteBatch(),
                                 m_camera->getTransform());
    }

    getWindow()->display();
}
