#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Generated/shaders/sprite.hpp>

#include <iostream>
#include <stdexcept>

#include "Graphics/SpriteBatch.hpp"
#include "Graphics/Window.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/Shader.hpp"

#include "Utils/Camera2D.hpp"
#include "Utils/Colors.hpp"
#include "Utils/Math.hpp"
#include "Utils/Strings.hpp"

namespace
{
    constexpr std::string_view title = "PacMan";
    constexpr std::string_view fpsString = "Avg FPS: %s";
    constexpr std::string_view delimeter = "%s | %s";

    constexpr int screenFps = 30;
    constexpr int screenTicksPerFrame = 1000 / screenFps;

    constexpr glm::ivec2 windowSize(1280, 720);

    std::string makeWindowTitle(const std::string_view& avgFps)
    {
        std::string result(title);
        if (!avgFps.empty())
        {
            const std::string fps = strings::format(fpsString, avgFps.data());
            result = strings::format(delimeter, result.c_str(), fps.c_str());
        }
        return result;
    }

    std::string toString(glm::vec4 vec4)
    {
        std::string result = "[";
        for (int i = 0; i < 4; ++i)
        {
            result += std::to_string(vec4[i]);
            if (i != 3)
            {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }
}

struct GameData
{
    std::shared_ptr<Camera2D> camera = nullptr;
    glm::vec4 bgColor {};
    glm::vec4 sourceRect = glm::vec4(0, 0, 32, 32);
};

Game::Game()
    : m_isRunning(false)
    , m_data(std::make_unique<GameData>())
    , m_resources(nullptr)
    , m_window(nullptr)
    , m_spriteBatch(nullptr)
{
}

Game::~Game() = default;

void Game::run()
{
    if (!m_isRunning)
    {
        init();
        loadResource();

        uint32_t totalFrames = 0;
        GameTime gameTime;
        while (m_isRunning)
        {
            totalFrames++;
            const uint64_t start = SDL_GetPerformanceCounter();

            handleEvents();
            gameTime.beginUpdate();
            update(gameTime);
            gameTime.endUpdate();
            draw();

            const uint64_t end = SDL_GetPerformanceCounter();
            if (m_window)
            {
                m_window->setTitle(makeWindowTitle(std::to_string(totalFrames / gameTime.getTotalMs())));

                const float elapsedMs = (end - start) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f;
                if (screenTicksPerFrame > elapsedMs)
                {
                    SDL_Delay(static_cast<uint32_t>(screenTicksPerFrame - elapsedMs));
                }
            }

            if (totalFrames > 20000)
            {
                totalFrames = 0;
            }
        }

        unloadResource();
    }
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error(strings::format("SDL Init Error: %s", SDL_GetError()));
    }

    constexpr int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
    {
        throw std::runtime_error(strings::format("IMG Init Error: %s", IMG_GetError()));
    }

    // Init window
    m_window = std::make_unique<Window>(title, windowSize.x, windowSize.y);

    // Init resources
    m_resources = ResourceManager::create();
    m_resources->addSearchPath("sprites.bundle", "sprites");
    m_resources->addSearchPath(".", "lol");
    m_resources->addFile<Texture2D>("pacman", "sprites/pacman.png");
    m_resources->addFile<Texture2D>("wall", "lol/wall.jpg");

    // Init renderer and camera
    if (!m_resources->has<Shader>("sprite"))
    {
        const auto& spriteShader
            = m_resources->loadOrCreate<Shader>("sprite", shaders::sprite_vert, shaders::sprite_frag);
        if (spriteShader)
        {
            m_spriteBatch = std::make_unique<SpriteBatch>(spriteShader);
        }
    }
    m_data->camera = std::make_shared<Camera2D>(windowSize, 0.5f);

    m_data->bgColor = colors::toGL(0x3AB4F2FF);

    m_isRunning = true;
}

void Game::loadResource() { }

void Game::unloadResource() { m_resources->unload<Texture2D>("pacman"); }

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
            m_data->camera->setWindowSize(m_window->getWindowSize());
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            constexpr float scalar = 0.1f;
            const glm::vec2& cameraScale = m_data->camera->getScale();
            if (e.wheel.y > 0)
            {
                m_data->camera->setScale(cameraScale + scalar);
            }
            else if (e.wheel.y < 0)
            {
                m_data->camera->setScale(cameraScale - scalar);
            }
        }
    }

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

void Game::update(const GameTime& gameTime) { }

void Game::draw()
{
    m_window->clear(m_data->bgColor);

    m_spriteBatch->begin(m_data->camera->getTransform());
    m_spriteBatch->draw(m_resources->load<Texture2D>("pacman"), colors::toGL(0x6FEDD6ff), glm::vec4(0, 0, 32, 32),
                        m_data->sourceRect, math::degreesToRadians(90.0f), glm::vec2(0, 32));
    m_spriteBatch->end();

    m_window->display();
}
