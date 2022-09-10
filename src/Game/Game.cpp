#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Generated/shaders/sprite.hpp>

// inari
#include "Inari/ECS/Components/Sprite.hpp"
#include "Inari/ECS/Components/Transformation.hpp"
#include "Inari/ECS/EntityRegistry.hpp"

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
    constexpr glm::vec4 sourceRects[]
        = { glm::vec4(0, 0, 32, 32),   glm::vec4(32, 0, 64, 32),   glm::vec4(64, 0, 96, 32),
            glm::vec4(96, 0, 128, 32), glm::vec4(128, 0, 160, 32), glm::vec4(160, 0, 192, 32) };
}

Game::Game()
    : m_entityRegistry(std::make_unique<EntityRegistry>())
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

        return true;
    }
    return false;
}

void Game::loadResources()
{
    EntityPtr pacman = m_entityRegistry->createEntity("pacman");
    m_entityRegistry->emplaceComponent<Sprite>(pacman, m_resources->load<Texture2D>("pacman"), colors::White,
                                               glm::vec2(32), Constants::sourceRects[trackIdx]);
    m_entityRegistry->emplaceComponent<Transformation>(pacman, glm::vec2(0), 0.0f, glm::vec2(0));
}

void Game::unloadResources() { m_resources->unload<Texture2D>("pacman"); }

void Game::handleWindowResized(const glm::ivec2& size) { m_camera->setWindowSize(m_window->getWindowSize()); }

void Game::update(const GameTime& gameTime)
{
    constexpr float cameraSpeed = 1.0f;
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

    // todo make animation system
    EntityPtr entity = m_entityRegistry->getEntity("pacman");
    if (entity)
    {
        auto* sprite = m_entityRegistry->getComponent<Sprite>(entity);
        if (sprite)
        {
            trackIdx++;
            if (trackIdx == 6)
            {
                trackIdx = 0;
            }
            sprite->sourceRect = Constants::sourceRects[trackIdx];
        }
    }
}

void Game::draw(const GameTime& gameTime)
{
    m_window->clear(Constants::bgColor);

    m_spriteBatch->begin(m_camera->getTransform());
    for (const auto& entity : m_entityRegistry->getEntities())
    {
        if (entity)
        {
            const auto* sprite = m_entityRegistry->getComponent<Sprite>(entity);
            const auto* transformation = m_entityRegistry->getComponent<Transformation>(entity);
            if (sprite && transformation)
            {
                const glm::vec4 destRect(transformation->position, sprite->size);
                m_spriteBatch->draw(sprite->texture, sprite->color, destRect, sprite->sourceRect,
                                    transformation->radian, transformation->origin);
            }
        }
    }
    m_spriteBatch->end();

    m_window->display();
}
