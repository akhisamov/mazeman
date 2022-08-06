#include "Game.hpp"

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <stdexcept>

#include "Graphics/Sprite.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Graphics/Window.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/Shader.hpp"

#include "Utils/Colors.hpp"
#include "Utils/Strings.hpp"

#include "Camera2D.hpp"

namespace
{
    constexpr std::string_view title = "PacMan";
    constexpr std::string_view fpsString = "Avg FPS: %s";
    constexpr std::string_view delimeter = "%s | %s";

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

    constexpr int screenFps = 30;
    constexpr int screenTicksPerFrame = 1000 / screenFps;
}

struct GameData
{
    std::shared_ptr<Sprite> sprite = nullptr;
    std::shared_ptr<Camera2D> camera = nullptr;
    glm::vec4 bgColor;
};

Game::Game()
    : m_isRunning(false)
    , m_data(std::make_unique<GameData>())
    , m_resources(nullptr)
    , m_window(nullptr)
    , m_renderer(nullptr)
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
                    SDL_Delay(screenTicksPerFrame - elapsedMs);
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
    constexpr int width = 1280;
    constexpr int height = 720;
    m_window = std::make_unique<Window>(title, width, height);

    // Init renderer and camera
    m_renderer = std::make_unique<SpriteRenderer>();
    m_data->camera = std::make_shared<Camera2D>();
    m_data->camera->setWindowSize(width, height);

    // Init resources
    m_resources = ResourceManager::create();
    m_resources->addSearchPath("res.bundle", "res");
    m_resources->addFile<Texture2D>("wall", "res/textures/wall.jpg");

    m_data->bgColor = colors::toGL(0x3AB4F2);

    m_isRunning = true;
}

void Game::loadResource()
{
    const auto& texture = m_resources->load<Texture2D>("wall");
    m_data->sprite = Sprite::create(texture);
    m_data->sprite->setOrigin(0.0f, 0.0f);
}

void Game::unloadResource()
{
    m_resources->unload<Texture2D>("wall");
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
            const Window::Size windowSize = m_window->getWindowSize();
            m_data->camera->setWindowSize(windowSize.width, windowSize.height);
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            constexpr float scalar = 0.1f;
            const float cameraScale = m_data->camera->getScale();
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
}

void Game::update(const GameTime& gameTime) { }

void Game::draw()
{
    m_renderer->begin(m_data->bgColor, m_data->camera);
    m_renderer->draw(m_data->sprite);
    m_renderer->end();

    m_window->display();
}
