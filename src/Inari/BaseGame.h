#pragma once

#include <glm/ext/vector_int2.hpp>

#include <memory>
#include <string>

namespace inari {
    class Window;
    class Renderer;
    class SpriteBatch;
    class InputManager;
    class GameTime;
    class AssetsManager;

    class BaseGame {
    public:
        void run();

    protected:
        virtual bool init();

        virtual void loadResources() = 0;
        virtual void unloadResources() = 0;

        virtual void handleWindowResized(const glm::ivec2& size) = 0;

        virtual void update(const GameTime& gameTime) = 0;
        virtual void draw(const GameTime& gameTime) = 0;

    public:
        BaseGame();

        BaseGame(const BaseGame&) = delete;
        BaseGame(BaseGame&&) = delete;
        BaseGame& operator=(const BaseGame&) = delete;
        BaseGame& operator=(BaseGame&&) = delete;

    private:
        void handleEvents();
        void clear();

        bool m_isRunning;

        std::shared_ptr<Window> m_window;
        std::shared_ptr<Renderer> m_renderer;
        std::shared_ptr<SpriteBatch> m_spriteBatch;
        std::shared_ptr<InputManager> m_inputManager;

        std::shared_ptr<AssetsManager> m_assets;
    };
}
