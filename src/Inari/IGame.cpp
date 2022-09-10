#include "IGame.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <Generated/shaders/sprite.hpp>

#include <iostream>

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"
#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Utils/GameTime.hpp"
#include "Inari/Utils/Strings.hpp"

namespace inari
{
    IGame::IGame()
        : m_isRunning(false)
        , m_window(nullptr)
        , m_spriteBatch(nullptr)
        , m_resources(nullptr)
    {
    }

    void IGame::run()
    {
        if (!m_isRunning)
        {
            if (init())
            {
                loadResources();

                uint32_t totalFrames = 0;
                GameTime gameTime;
                while (m_isRunning)
                {
                    totalFrames++;
                    m_window->begin();

                    handleEvents();

                    update(gameTime.getTotalMs());
                    draw(gameTime.getTotalMs());

                    m_window->end();

                    const float avgFPS = totalFrames / gameTime.getTotalMs(); // todo
                    if (totalFrames > 20000)
                    {
                        totalFrames = 0;
                    }
                }

                unloadResources();
            }
        }
    }

    bool IGame::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        constexpr int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
        if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
        {
            std::cerr << "IMG Init Error: " << IMG_GetError() << std::endl;
            return false;
        }

        // Init window
        m_window = Window::create("Inari", 1280, 720);

        // Init resources
        m_resources = ResourceManager::create();
        if (m_resources == nullptr)
        {
            return false;
        }

        // Init renderer
        const auto& spriteShader
            = m_resources->loadOrCreate<Shader>("sprite", shaders::sprite_vert, shaders::sprite_frag);
        if (spriteShader)
        {
            m_spriteBatch = std::make_unique<SpriteBatch>(spriteShader);
        }
        else
        {
            return false;
        }

        m_isRunning = true;

        return true;
    }

    void IGame::handleEvents()
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
                handleWindowResized(m_window->getWindowSize());
            }
        }
    }
}