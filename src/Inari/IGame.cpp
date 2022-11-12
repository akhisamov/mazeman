#include "IGame.hpp"

#include <SDL.h>

#include <Generated/shaders/sprite.hpp>

#include <iostream>

#include "Inari/Graphics/SpriteBatch.hpp"
#include "Inari/Graphics/Window.hpp"
#include "Inari/InputManager.hpp"
#include "Inari/Resources/ResourceManager.hpp"
#include "Inari/Resources/Shader.hpp"
#include "Inari/Utils/GameTime.hpp"
#include "Inari/Utils/Strings.hpp"

namespace inari {
IGame::IGame()
    : m_isRunning(false),
      m_window(nullptr),
      m_spriteBatch(nullptr),
      m_resources(nullptr),
      m_inputManager(std::make_shared<InputManager>()) {}

void IGame::run() {
    if (!m_isRunning) {
        assert(init() && "Init is failed");
        loadResources();

        uint32_t totalFrames = 0;
        GameTime gameTime;
        while (m_isRunning) {
            totalFrames++;
            m_window->begin();

            handleEvents();

            const float dt = gameTime.getElapsedGameTime();
            update(dt);
            draw(dt);

            m_window->end();

            // const float avgFPS = totalFrames / gameTime.getTotalMs(); // TODO
            if (totalFrames > 20000) {
                totalFrames = 0;
            }

            gameTime.reset();
        }

        unloadResources();
    }
}

bool IGame::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Init window
    m_window = Window::create("Inari", 1280, 720);

    // Init resources
    m_resources = ResourceManager::create();
    if (m_resources == nullptr) {
        return false;
    }
    m_resources->addSearchPath(".");
    m_resources->addFileData("sprite", shaders::sprite);

    // Init renderer
    const auto& spriteShader = m_resources->load<Shader>("sprite");
    if (spriteShader == nullptr) {
        return false;
    }
    m_spriteBatch = std::make_shared<SpriteBatch>(spriteShader);

    m_isRunning = true;

    return true;
}

const std::shared_ptr<ResourceManager>& IGame::getResourceManager() const {
    return m_resources;
}

const std::shared_ptr<SpriteBatch>& IGame::getSpriteBatch() const {
    return m_spriteBatch;
}

const std::shared_ptr<Window>& IGame::getWindow() const {
    return m_window;
}

const std::shared_ptr<InputManager>& IGame::getInputManager() const {
    return m_inputManager;
}

void IGame::handleEvents() {
    m_inputManager->prepareHandling();
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            m_isRunning = false;
        } else if (e.type == SDL_WINDOWEVENT &&
                   e.window.event == SDL_WINDOWEVENT_RESIZED) {
            handleWindowResized(m_window->getWindowSize());
        } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            m_inputManager->handleEvent(e.key);
        }
    }
}
}  // namespace inari
