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
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transformation.hpp"
#include "Inari/ECS/EntityRegistry.hpp"
#include "Inari/ECS/SystemRegistry.hpp"
#include "Inari/ECS/Systems/AnimationSystem.hpp"
#include "Inari/ECS/Systems/SpriteRenderSystem.hpp"

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"

#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Resources/Shader.hpp"

#include "Inari/Utils/Camera2D.hpp"
#include "Inari/Utils/Colors.hpp"
#include "Inari/Utils/Math.hpp"
#include "Inari/Utils/Strings.hpp"
// inari

using namespace inari;

namespace Constants
{
    constexpr std::string_view title = "PacMan";
    constexpr int screenFps = 30;
    constexpr glm::ivec2 windowSize(1280, 720);
    constexpr glm::vec4 bgColor = colors::Black;
}

struct TracksGenerator
{
    glm::vec4 operator()()
    {
        auto result = glm::vec4(0, 0, 32, 32);
        result.x = static_cast<float>(i++) * 32.0f;
        result.z = static_cast<float>(i) * 32.0f;
        return result;
    }

private:
    int i = 0;
};

Game::Game()
    : m_entityRegistry(std::make_shared<EntityRegistry>())
    , m_systemRegistry(std::make_unique<SystemRegistry>())
    , m_camera(nullptr)
{
}

Game::~Game() = default;

bool Game::init()
{
    if (IGame::init())
    {
        // Init window
        m_window->setWindowSize(Constants::windowSize);
        m_window->setTitle(Constants::title);
        m_window->setFrameLimit(Constants::screenFps);

        // Init resources
        m_resources->addSearchPath("sprites.bundle", "sprites");
        m_resources->addFile<Texture2D>("pacman", "sprites/pacman.png");

        // Init camera
        m_camera = std::make_unique<Camera2D>(Constants::windowSize, 0.5f);

        // Init systems
        m_systemRegistry->addSystem<AnimationSystem>(m_entityRegistry);
        m_systemRegistry->addSystem<SpriteRenderSystem>(m_entityRegistry);

        return true;
    }
    return false;
}

void Game::loadResources()
{
    EntityPtr pacman = m_entityRegistry->createEntity("pacman");
    m_entityRegistry->emplaceComponent<Transformation>(pacman, glm::vec2(0), 0.0f, glm::vec2(0));
    m_entityRegistry->emplaceComponent<Sprite>(pacman, m_resources->load<Texture2D>("pacman"), glm::vec2(32));
    if (auto* animSprite = m_entityRegistry->emplaceComponent<AnimationSprite>(pacman, "default"))
    {
        auto& defaultTracks = animSprite->tracks["default"];
        defaultTracks.resize(6);
        std::generate(defaultTracks.begin(), defaultTracks.end(), TracksGenerator());

        // Uncomment it to limit animation fps
        // animSprite->isFramesLimited = true;
        // animSprite->framesLimit = 24.0f;
    }
}

void Game::unloadResources() { m_resources->unload<Texture2D>("pacman"); }

void Game::handleWindowResized(const glm::ivec2& size) { m_camera->setWindowSize(m_window->getWindowSize()); }

void Game::update(float dt)
{
    const float cameraSpeed = 1.0f * dt;
    const uint8_t* currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        m_camera->moveY(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        m_camera->moveY(-cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        m_camera->moveX(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        m_camera->moveX(-cameraSpeed);
    }

    auto animation = m_systemRegistry->getSystem<AnimationSystem>();
    if (animation)
    {
        animation->update(dt);
    }
}

void Game::draw(float dt)
{
    m_window->clear(Constants::bgColor);

    auto spriteRenderSystem = m_systemRegistry->getSystem<SpriteRenderSystem>();
    if (spriteRenderSystem)
    {
        spriteRenderSystem->draw(dt, m_spriteBatch, m_camera->getTransform());
    }

    m_window->display();
}
