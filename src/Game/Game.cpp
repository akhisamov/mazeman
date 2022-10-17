#include "Game.hpp"

#include <SDL_keycode.h>

// inari
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transform.hpp"
#include "Inari/ECS/EntityRegistry.hpp"
#include "Inari/ECS/SystemRegistry.hpp"
#include "Inari/ECS/Systems/AnimationSystem.hpp"
#include "Inari/ECS/Systems/PhysicsSystem.hpp"
#include "Inari/ECS/Systems/SpriteRenderSystem.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"

#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Resources/Texture2D.hpp"
#include "Inari/Resources/World.hpp"

#include "Inari/Utils/Camera2D.hpp"
#include "Inari/Utils/Colors.hpp"

#include "Inari/InputManager.hpp"
// inari

// game
#include "Game/Components/Player.hpp"
#include "Game/Prefabs/Pacman.hpp"
#include "Game/Systems/InputSystem.hpp"
// game

namespace constants {
constexpr std::string_view title = "PacMan";
constexpr int screenFps = 30;
constexpr glm::ivec2 windowSize(1280, 720);
}  // namespace constants

Game::Game()
    : m_entityRegistry(std::make_shared<inari::EntityRegistry>()),
      m_systemRegistry(std::make_unique<inari::SystemRegistry>()),
      m_camera(nullptr) {}

Game::~Game() = default;

bool Game::init() {
    if (IGame::init()) {
        // Init window
        const auto& window = getWindow();
        window->setWindowSize(constants::windowSize);
        window->setTitle(constants::title);
        window->setFrameLimit(constants::screenFps);

        // Init camera
        m_camera =
            std::make_unique<inari::Camera2D>(constants::windowSize, 1.1f);

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
    auto world = getResourceManager()->load<inari::World>("res/pacman.ldtk");
    if (world) {
        const inari::WorldLevel& level = world->getLevel(0);

        {
            auto it = level.layers.find("Walls");
            for (const auto& tile : it->second.gridTiles) {
                inari::Sprite sprite;
                sprite.texture = getResourceManager()->load<inari::Texture2D>(
                    "res/walls.png");
                sprite.size = glm::vec2(tile.sourceRect.w, tile.sourceRect.z);
                sprite.sourceRect = tile.sourceRect;

                inari::Transform transform;
                transform.position = tile.position;

                auto tileEntity = m_entityRegistry->createEntity();
                m_entityRegistry->emplaceComponent(tileEntity, sprite);
                m_entityRegistry->emplaceComponent(tileEntity, transform);
            }
        }

        {
            auto it = level.layers.find("Spawns")->second.entityInstances.find(
                "Pacman");
            const inari::LevelEntityInstance entityInstance = it->second;
            prefabs::createPacman(
                m_entityRegistry,
                getResourceManager()->load<inari::Texture2D>("res/pacman.png"),
                entityInstance.position, entityInstance.get<float>("angle"));
        }
    }
}

void Game::unloadResources() {
    getResourceManager()->unload<inari::Texture2D>("res/pacman.png");
}

void Game::handleWindowResized(const glm::ivec2& size) {
    m_camera->setWindowSize(size);
}

void Game::update(float dt) {
    m_systemRegistry->updateSystem<InputSystem>(dt);
    m_systemRegistry->updateSystem<inari::PhysicsSystem>(dt);
    m_systemRegistry->updateSystem<inari::AnimationSystem>(dt);

    if (getInputManager()->isKeyPressed(SDLK_F1)) {
        getSpriteBatch()->toggleWireframeMode();
    }
}

void Game::draw(float dt) {
    glm::vec3 bgColor(0.0f);
    auto world = getResourceManager()->load<inari::World>("res/pacman.ldtk");
    if (world) {
        const inari::WorldLevel& level = world->getLevel(0);
        bgColor = level.backgroundColor;
    }
    getWindow()->clear(bgColor);

    auto spriteRenderSystem =
        m_systemRegistry->getSystem<inari::SpriteRenderSystem>();
    if (spriteRenderSystem) {
        spriteRenderSystem->draw(dt, getSpriteBatch(),
                                 m_camera->getTransform());
    }

    getWindow()->display();
}
