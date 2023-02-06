#include "IGame.h"

#include <SDL.h>

#include <iostream>

#include "Inari/Assets/AssetsManager.h"
#include "Inari/Assets/Shader.h"
#include "Inari/Assets/ShaderMaker.h"
#include "Inari/Assets/Texture2D.h"
#include "Inari/Assets/Texture2DMaker.h"
#include "Inari/Assets/World.h"
#include "Inari/Assets/WorldMaker.h"
#include "Inari/GameServices.h"
#include "Inari/Graphics/Renderer.hpp"
#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"
#include "Inari/InputManager.hpp"
#include "Inari/Utils/GameTime.hpp"

namespace inari {
    IGame::IGame()
        : m_isRunning(false)
        , m_window(nullptr)
        , m_renderer(nullptr)
        , m_spriteBatch(nullptr)
        , m_assets(nullptr)
        , m_inputManager(std::make_shared<InputManager>())
    {
    }

    void IGame::run()
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
        }
    }

    bool IGame::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Init window
        m_window = Window::create("Inari", 1280, 720);

        // Init renderer
        m_renderer = std::make_shared<Renderer>();

        // Init assets
        m_assets = AssetsManager::create();
        if (m_assets == nullptr) {
            return false;
        }
        m_assets->addSearchPath(".");
        m_assets->registerMaker<Texture2D>(std::make_shared<Texture2DMaker>());
        m_assets->registerMaker<Shader>(std::make_shared<ShaderMaker>());
        m_assets->registerMaker<World>(std::make_shared<WorldMaker>());
        GameServices::provide(m_assets);

        // Init sprite batch
        m_spriteBatch = std::make_shared<SpriteBatch>(m_renderer);

        m_isRunning = true;

        return true;
    }

    const std::shared_ptr<Renderer>& IGame::getRenderer() const { return m_renderer; }

    const std::shared_ptr<SpriteBatch>& IGame::getSpriteBatch() const { return m_spriteBatch; }

    const std::shared_ptr<Window>& IGame::getWindow() const { return m_window; }

    const std::shared_ptr<InputManager>& IGame::getInputManager() const { return m_inputManager; }

    void IGame::handleEvents()
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
}
