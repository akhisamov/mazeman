#include "Game.hpp"

// inari
#include "Inari/ECS/EntityRegistry.hpp"
#include "Inari/ECS/SystemRegistry.hpp"
#include "Inari/ECS/Systems/AnimationSystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"
#include "Inari/ECS/Systems/SpriteRenderSystem.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"

#include "Inari/Resources/ResourceManager.hpp"

#include "Inari/Utils/Camera2D.hpp"
#include "Inari/Utils/Colors.hpp"
// inari

// game
#include "Game/Components/Player.hpp"
#include "Game/Prefabs/Pacman.hpp"
#include "Game/Systems/InputSystem.hpp"
// game

namespace Constants {
constexpr std::string_view title = "PacMan";
constexpr int screenFps = 30;
constexpr glm::ivec2 windowSize(1280, 720);
constexpr glm::vec4 bgColor = inari::colors::Black;
}  // namespace Constants

Game::Game()
    : m_entityRegistry(std::make_shared<inari::EntityRegistry>()),
      m_systemRegistry(std::make_unique<inari::SystemRegistry>()),
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
        resources->addFile<inari::Texture2D>("pacman", "sprites/pacman.png");

        // Init camera
        m_camera =
            std::make_unique<inari::Camera2D>(Constants::windowSize, 0.5f);

        // Init systems
        m_systemRegistry->addSystem<inari::AnimationSystem>(m_entityRegistry);
        m_systemRegistry->addSystem<inari::SpriteRenderSystem>(
            m_entityRegistry);
        m_systemRegistry->addSystem<inari::PhysicsSystem>(m_entityRegistry);
        m_systemRegistry->addSystem<InputSystem>(m_entityRegistry,
                                                 getInputManager());

        return true;
    }
    return false;
}

void Game::loadResources() {
    prefabs::createPacman(
        m_entityRegistry,
        getResourceManager()->load<inari::Texture2D>("pacman"));
}

void Game::unloadResources() {
    getResourceManager()->unload<inari::Texture2D>("pacman");
}

void Game::handleWindowResized(const glm::ivec2& size) {
    m_camera->setWindowSize(size);
}

void Game::update(float dt) {
    m_systemRegistry->updateSystem<InputSystem>(dt);
    m_systemRegistry->updateSystem<inari::PhysicsSystem>(dt);
    m_systemRegistry->updateSystem<inari::AnimationSystem>(dt);
}

void Game::draw(float dt) {
    getWindow()->clear(Constants::bgColor);

    auto spriteRenderSystem =
        m_systemRegistry->getSystem<inari::SpriteRenderSystem>();
    if (spriteRenderSystem) {
        spriteRenderSystem->draw(dt, getSpriteBatch(),
                                 m_camera->getTransform());
    }

    getWindow()->display();
}
