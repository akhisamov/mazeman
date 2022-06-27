#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <glm/ext/matrix_clip_space.hpp>

#include <stdexcept>

#include "ResourceManager/ResourceManager.hpp"

#include "Resources/Shader.hpp"

#include "SpriteRenderer.hpp"
#include "Sprite.hpp"
#include "StringUtils.hpp"
#include "Window.hpp"

struct GameData
{
    std::shared_ptr<Sprite> sprite = nullptr;
};

Game::Game() :
        m_isRunning(false),
        m_data(std::make_unique<GameData>()),
        m_window(nullptr),
        m_resources(nullptr),
        m_renderer(nullptr)
{
}

Game::~Game()
{
}

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
    m_resources = std::make_unique<ResourceManager>(bundleFile);

    const auto& spriteShader = m_resources->load<Shader>("sprite");
    spriteShader->use();
    spriteShader->set("projection", glm::ortho(0.0f,
            static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f));
    m_renderer = std::make_unique<SpriteRenderer>(spriteShader);

    m_isRunning = true;
}

void Game::loadResource()
{
    m_data->sprite = Sprite::create(m_resources->load<Texture2D>("wall"));
    m_data->sprite->setOrigin(0.0f, 0.0f);
}

void Game::unloadResource()
{
    m_resources->unload(m_data->sprite->getTexture());
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
    }
}

void Game::draw()
{
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_renderer->draw(m_data->sprite);

    m_window->display();
}