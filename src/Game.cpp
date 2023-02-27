#include "Game.h"

#include <SDL_keycode.h>

// inari
#include "Inari/Common/Color.h"

#include "Inari/ECS/Components/Sprite.h"
#include "Inari/ECS/Components/Transform.h"
#include "Inari/ECS/EntityRegistry.h"
#include "Inari/ECS/SystemRegistry.h"
#include "Inari/ECS/Systems/AnimationSystem.h"
#include "Inari/ECS/Systems/PhysicsSystem.h"
#include "Inari/ECS/Systems/SpriteRenderSystem.h"

#include "Inari/Graphics/Renderer.h"
#include "Inari/Graphics/SpriteBatch.h"
#include "Inari/Graphics/Window.h"

#include "Inari/Assets/AssetsManager.h"
#include "Inari/Assets/Texture2D.h"
#include "Inari/Assets/World.h"

#include "Inari/Utils/Camera2D.h"

#include "Inari/GameServices.h"
#include "Inari/InputManager.h"
// inari

// game
#include "Components/Collision.h"
#include "Prefabs/Mazeman.h"
#include "Systems/CollisionSystem.h"
#include "Systems/InputSystem.h"
// game

namespace constants {
    constexpr std::string_view title = "MazeMan";
    constexpr int screenFps = 30;
    constexpr inari::Vec2i windowSize(1280, 720);
    constexpr std::string_view worldFilename = "res/world.ldtk";
} // namespace constants

inari::Color getBackgroundColor()
{
    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets) {
        auto world = assets->load<inari::World>(constants::worldFilename);
        if (world) {
            const inari::WorldLevel& level = world->getLevel(0);
            return level.backgroundColor;
        }
    }
    return inari::Color::White();
}

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
        const auto& window = inari::GameServices::get<inari::Window>();
        window->setWindowSize(constants::windowSize);
        window->setTitle(constants::title);
        window->setFrameLimit(constants::screenFps);

        // Init camera
        m_camera = std::make_unique<inari::Camera2D>(constants::windowSize);

        // Init systems
        m_systemRegistry->addSystem<inari::SpriteRenderSystem>();
        m_systemRegistry->addOrderedSystem<InputSystem>();
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
        m_camera->setScale(inari::Vec2f(level.size.y / m_camera->getWindowSize().y));

        {
            auto it = level.layers.find("Collisions");
            for (const auto& tile : it->second.tiles) {
                inari::Sprite sprite;
                sprite.texture = assets->load<inari::Texture2D>("res/walls.png");
                sprite.sourceRect = tile.sourceRect;

                inari::Transform transform;
                transform.position = tile.position;
                transform.size = inari::Vec2f(tile.sourceRect.w, tile.sourceRect.z);

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

void Game::handleWindowResized(const inari::Vec2i& size)
{
    m_camera->setWindowSize(size);

    const auto& assets = inari::GameServices::get<inari::AssetsManager>();
    if (assets == nullptr) {
        return;
    }

    auto world = assets->load<inari::World>(constants::worldFilename);
    if (world) {
        const inari::WorldLevel& level = world->getLevel(0);
        m_camera->setScale(inari::Vec2f(level.size.y / m_camera->getWindowSize().y));
    }
}

void Game::update(const inari::GameTime& gameTime)
{
    m_systemRegistry->updateOrderedSystem(gameTime, m_entityRegistry);
    if (inari::GameServices::get<inari::InputManager>()->isKeyPressed(SDLK_F1)) {
        inari::GameServices::get<inari::SpriteBatch>()->toggleWireframeMode();
    }
}

void Game::draw(const inari::GameTime& gameTime)
{
    inari::GameServices::get<inari::Renderer>()->clear(getBackgroundColor());

    auto spriteRenderSystem = m_systemRegistry->getSystem<inari::SpriteRenderSystem>();
    if (spriteRenderSystem) {
        spriteRenderSystem->begin(m_camera->getTransform());
        m_systemRegistry->updateSystem(spriteRenderSystem, gameTime, m_entityRegistry);
        spriteRenderSystem->end();
    }

    inari::GameServices::get<inari::Window>()->display();
}
