﻿#include "BaseGame.h"

#include <SDL.h>

#include <iostream>

#include "Inari/Assets/AssetsManager.h"
#include "Inari/GameServices.h"
#include "Inari/Graphics/Renderer.hpp"
#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"
#include "Inari/InputManager.h"
#include "Inari/Utils/GameTime.h"

namespace inari {
    BaseGame::BaseGame()
        : m_isRunning(false)
        , m_window(nullptr)
        , m_renderer(nullptr)
        , m_spriteBatch(nullptr)
        , m_assets(nullptr)
        , m_inputManager(std::make_shared<InputManager>())
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

        // Init renderer
        m_renderer = std::make_shared<Renderer>();

        // Init assets
        m_assets = AssetsManager::create();
        if (m_assets == nullptr) {
            return false;
        }
        GameServices::provide(m_assets);

        // Init sprite batch
        m_spriteBatch = std::make_shared<SpriteBatch>(m_renderer);

        m_isRunning = true;

        return true;
    }

    const std::shared_ptr<Renderer>& BaseGame::getRenderer() const { return m_renderer; }

    const std::shared_ptr<SpriteBatch>& BaseGame::getSpriteBatch() const { return m_spriteBatch; }

    const std::shared_ptr<Window>& BaseGame::getWindow() const { return m_window; }

    const std::shared_ptr<InputManager>& BaseGame::getInputManager() const { return m_inputManager; }

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
}