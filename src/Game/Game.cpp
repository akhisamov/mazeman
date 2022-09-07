#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Generated/shaders/sprite.hpp>

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"
#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Resources/Shader.hpp"
#include "Inari/Utils/Camera2D.hpp"
#include "Inari/Utils/Colors.hpp"
#include "Inari/Utils/Math.hpp"
#include "Inari/Utils/Strings.hpp"

namespace
{
    constexpr std::string_view title = "PacMan";
    constexpr int screenFps = 30;
    constexpr glm::ivec2 windowSize(1280, 720);
}

struct GameData
{
    std::shared_ptr<inari::Camera2D> camera = nullptr;
    glm::vec4 bgColor = glm::vec4(0, 0, 0, 1);
    glm::vec4 sourceRect = glm::vec4(0, 0, 32, 32);
};

Game::Game()
    : m_data(std::make_unique<GameData>())
{
}

Game::~Game() = default;

bool Game::init()
{
    if (IGame::init())
    {
        // Init window
        m_window->setWindowSize(windowSize);
        m_window->setTitle(title);
        m_window->setFrameLimit(screenFps);

        // Init resources
        m_resources->addSearchPath("sprites.bundle", "sprites");
        m_resources->addFile<inari::Texture2D>("pacman", "sprites/pacman.png");

        // Init camera
        m_data->camera = std::make_shared<inari::Camera2D>(windowSize, 0.5f);

        return true;
    }
    return false;
}

void Game::loadResources() { }

void Game::unloadResources() { m_resources->unload<inari::Texture2D>("pacman"); }

void Game::handleWindowResized(const glm::ivec2& size) { m_data->camera->setWindowSize(m_window->getWindowSize()); }

void Game::update(const inari::GameTime& gameTime)
{
    constexpr float cameraSpeed = 1.0f;
    const uint8_t* currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        m_data->camera->moveY(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        m_data->camera->moveY(-cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        m_data->camera->moveX(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        m_data->camera->moveX(-cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_SPACE])
    {
        m_data->sourceRect.x += 32;
        m_data->sourceRect.z += 32;
        if (m_data->sourceRect.z >= 192)
        {
            m_data->sourceRect.x = 0;
            m_data->sourceRect.z = 32;
        }
    }
}

void Game::draw()
{
    m_window->clear(m_data->bgColor);

    m_spriteBatch->begin(m_data->camera->getTransform());
    m_spriteBatch->draw(m_resources->load<inari::Texture2D>("pacman"), glm::vec4(1), glm::vec4(0, 0, 32, 32),
                        m_data->sourceRect, inari::math::degreesToRadians(90.0f), glm::vec2(0, 32));
    m_spriteBatch->end();

    m_window->display();
}
