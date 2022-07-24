#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <stdexcept>

#include "Graphics/Sprite.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Graphics/Window.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/Shader.hpp"

#include "Camera2D.hpp"
#include "StringUtils.hpp"

struct GameData
{
    std::shared_ptr<Sprite> sprite = nullptr;
    std::shared_ptr<Camera2D> camera = nullptr;
};

std::unique_ptr<Game> Game::create() { return std::unique_ptr<Game>(new Game()); }

Game::Game()
    : m_isRunning(false)
    , m_data(std::make_unique<GameData>())
    , m_resources(nullptr)
    , m_window(nullptr)
    , m_renderer(nullptr)
{
}

Game::~Game() { }

void Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error(StringUtils::format("SDL Init Error: %s", SDL_GetError()));
    }

    constexpr int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
    {
        throw std::runtime_error(StringUtils::format("IMG Init Error: %s", IMG_GetError()));
    }

    constexpr std::string_view title = "Pac Man";
    constexpr int width = 1280;
    constexpr int height = 720;
    m_window = std::make_unique<Window>(title, width, height);

    constexpr std::string_view bundleFile = "res.bundle";
    m_resources = ResourceManager::create({ bundleFile });

    m_renderer = std::make_unique<SpriteRenderer>(m_window->getWindowSize());
    m_data->camera = std::make_shared<Camera2D>();

    m_isRunning = true;
}

void Game::loadResource()
{
    const auto& texture = m_resources->load<Texture2D>("textures/wall");
    m_data->sprite = Sprite::create(texture);
    m_data->sprite->setOrigin(0.0f, 0.0f);
}

void Game::unloadResource()
{
    m_resources->unload<Texture2D>("textures/wall");
    m_data->sprite = nullptr;
}

void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            m_isRunning = false;
        }
        else if (e.type == SDL_WINDOWEVENT_RESIZED)
        {
            m_renderer->setSize(m_window->getWindowSize());
        }
    }

    constexpr float cameraSpeed = 1.0f;
    const uint8_t* currentKeyStates = SDL_GetKeyboardState(nullptr);
    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        m_data->camera->moveY(-cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        m_data->camera->moveY(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        m_data->camera->moveX(cameraSpeed);
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        m_data->camera->moveX(-cameraSpeed);
    }
}

void Game::draw()
{
    m_renderer->begin(m_data->camera);
    m_renderer->draw(m_data->sprite);
    m_renderer->end();

    m_window->display();
}
