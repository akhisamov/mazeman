#include "BaseGame.h"

#include <SDL.h>

#include <iostream>

#include "Inari/Assets/AssetsManager.h"
#include "Inari/GameServices.h"
#include "Inari/Graphics/Renderer.h"
#include "Inari/Graphics/SpriteBatch.h"
#include "Inari/Graphics/Window.h"
#include "Inari/InputManager.h"
#include "Inari/Utils/GameTime.h"

namespace {
    template <class T>
    void destroyService(std::shared_ptr<T>& service)
    {
        inari::GameServices::remove<T>();
        service = nullptr;
    }
}

namespace inari {
    BaseGame::BaseGame()
        : m_isRunning(false)
        , m_window(nullptr)
        , m_renderer(nullptr)
        , m_spriteBatch(nullptr)
        , m_assets(nullptr)
        , m_inputManager(nullptr)
    {
    }

    void BaseGame::run()
    {
        if (!m_isRunning) {
            assert(init() && "Init is failed");
            loadResources();

            // uint32_t totalFrames = 0;
            GameTime gameTime;
            while (m_isRunning) {
                // totalFrames++;
                m_window->begin();

                handleEvents();

                update(gameTime);
                draw(gameTime);

                m_window->end();

                // const float avgFPS = totalFrames / gameTime.getTotalMs(); // TODO
                // if (totalFrames > 20000) {
                //    totalFrames = 0;
                //}

                gameTime.reset();
            }

            unloadResources();

            clear();
        }
    }

    bool BaseGame::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Init window
        m_window = Window::create("Inari", 1280, 720);
        if (m_window == nullptr) {
            return false;
        }
        GameServices::provide(m_window);

        // Init renderer
        m_renderer = Renderer::create(m_window);
        if (m_renderer == nullptr) {
            return false;
        }
        GameServices::provide(m_renderer);

        // Init assets
        m_assets = AssetsManager::create();
        if (m_assets == nullptr) {
            return false;
        }
        GameServices::provide(m_assets);

        // Init sprite batch
        m_spriteBatch = SpriteBatch::create(m_renderer);
        if (m_spriteBatch == nullptr) {
            return false;
        }
        GameServices::provide(m_spriteBatch);

        // Init input manager
        m_inputManager = InputManager::create();
        if (m_inputManager == nullptr) {
            return false;
        }
        GameServices::provide(m_inputManager);

        m_isRunning = true;

        return true;
    }

    void BaseGame::handleEvents()
    {
        m_inputManager->prepareHandling();
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                m_isRunning = false;
            } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                handleWindowResized(m_window->getWindowSize());
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                m_inputManager->handleEvent(e.key);
            }
        }
    }

    void BaseGame::clear()
    {
        destroyService(m_inputManager);
        destroyService(m_spriteBatch);
        destroyService(m_assets);
        destroyService(m_renderer);
        destroyService(m_window);
        SDL_Quit();
    }
}
