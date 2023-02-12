#include "Game.h"

#include <SDL_keycode.h>

// inari
#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"
#include "Inari/ECS/SystemRegistry.h"
#include "Inari/ECS/Systems/AnimationSystem.h"
#include "Inari/ECS/Systems/PhysicsSystem.h"
#include "Inari/ECS/Systems/SpriteRenderSystem.h"

#include "Inari/Graphics/Renderer.hpp"
#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"

#include "Inari/Assets/AssetsManager.h"
#include "Inari/Assets/Texture2D.h"
#include "Inari/Assets/World.h"

#include "Inari/Utils/Camera2D.h"
#include "Inari/Utils/Colors.h"

#include "Inari/GameServices.h"
#include "Inari/InputManager.h"
// inari

// game
#include "Game/Components/Collision.h"
#include "Game/Prefabs/Mazeman.h"
#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/InputSystem.h"
// game

namespace constants {
    constexpr std::string_view title = "MazeMan";
    constexpr int screenFps = 30;
    constexpr glm::ivec2 windowSize(1280, 720);
    constexpr std::string_view worldFilename = "res/world.ldtk";
} // namespace constants

Game::Game()
    : m_entityRegistry(std::make_shared<inari::EntityRegistry>())
    , m_systemRegistry(std::make_unique<inari::SystemRegistry>())
    , m_camera(nullptr)
{
}

Game::~Game() = default;

bool Game::init()
{
    if (BaseGame::init()) {
        // Init window
        const auto& window = getWindow();
        window->setWindowSize(constants::windowSize);
        window->setTitle(constants::title);
        window->setFrameLimit(constants::screenFps);

        // Init camera
        m_camera = std::make_unique<inari::Camera2D>(constants::windowSize);

        // Init systems
        m_systemRegistry->addSystem<inari::SpriteRenderSystem>(getSpriteBatch());
        m_systemRegistry->addOrderedSystem<InputSystem>(getInputManager());
        m_systemRegistry->addOrderedSystem<CollisionSystem>();
        m_systemRegistry->addOrderedSystem<inari::PhysicsSystem>();
        m_systemRegistry->addOrderedSystem<inari::AnimationSystem>();

        return true;
    }
    return false;
}

void Game::loadResources()
{
    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets == nullptr) {
        return;
    }

    auto world = assets->load<inari::World>(constants::worldFilename);
    if (world) {
        const inari::WorldLevel& level = world->getLevel(0);
        m_camera->setScale(glm::vec2(level.size.y / m_camera->getWindowSize().y));

        {
            auto it = level.layers.find("Collisions");
            for (const auto& tile : it->second.tiles) {
                inari::Sprite sprite;
                sprite.texture = assets->load<inari::Texture2D>("res/walls.png");
                sprite.sourceRect = tile.sourceRect;

                inari::Transform transform;
                transform.position = tile.position;
                transform.size = glm::vec2(tile.sourceRect.w, tile.sourceRect.z);

                Collision collision;
                collision.isDynamic = false;

                auto tileEntity = m_entityRegistry->createEntity();
                m_entityRegistry->emplaceComponent(tileEntity, sprite);
                m_entityRegistry->emplaceComponent(tileEntity, transform);
                m_entityRegistry->emplaceComponent(tileEntity, collision);
            }
        }

        {
            auto it = level.layers.find("Spawns")->second.entityInstances.find("MazeMan");
            const inari::LevelEntityInstance entityInstance = it->second;
            prefabs::createMazeman(m_entityRegistry, assets->load<inari::Texture2D>("res/mazeman.png"),
                                   entityInstance.position, entityInstance.get<float>("angle"));
        }
    }
}

void Game::unloadResources()
{
    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets == nullptr) {
        return;
    }
    assets->unload("res/mazeman.png");
}

void Game::handleWindowResized(const glm::ivec2& size)
{
    m_camera->setWindowSize(size);

    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets == nullptr) {
        return;
    }

    auto world = assets->load<inari::World>(constants::worldFilename);
    if (world) {
        const inari::WorldLevel& level = world->getLevel(0);
        m_camera->setScale(glm::vec2(level.size.y / m_camera->getWindowSize().y));
    }
}

void Game::update(const inari::GameTime& gameTime)
{
    m_systemRegistry->updateOrderedSystem(gameTime, m_entityRegistry);
    if (getInputManager()->isKeyPressed(SDLK_F1)) {
        getSpriteBatch()->toggleWireframeMode();
    }
}

void Game::draw(const inari::GameTime& gameTime)
{
    glm::vec3 bgColor(0.0f);
    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets) {
        auto world = assets->load<inari::World>(constants::worldFilename);
        if (world) {
            const inari::WorldLevel& level = world->getLevel(0);
            bgColor = level.backgroundColor;
        }
    }
    getRenderer()->clear(bgColor);

    auto spriteRenderSystem = m_systemRegistry->getSystem<inari::SpriteRenderSystem>();
    if (spriteRenderSystem) {
        spriteRenderSystem->begin(m_camera->getTransform());
        m_systemRegistry->updateSystem(spriteRenderSystem, gameTime, m_entityRegistry);
        spriteRenderSystem->end();
    }

    getWindow()->display();
}
