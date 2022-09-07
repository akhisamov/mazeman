#pragma once

#include <glm/ext/vector_int2.hpp>

#include <memory>
#include <string>

namespace inari
{
    class SpriteBatch;
    class Window;
    class ResourceManager;
    class GameTime;

    class IGame
    {
    public:
        IGame();

        IGame(const IGame&) = delete;
        IGame(IGame&&) = delete;

        void run();

    protected:
        virtual bool init();

        virtual void loadResources() = 0;
        virtual void unloadResources() = 0;

        virtual void handleWindowResized(const glm::ivec2& size) = 0;

        virtual void update(const inari::GameTime& gameTime) = 0;
        virtual void draw() = 0;

        std::unique_ptr<Window> m_window;
        std::unique_ptr<SpriteBatch> m_spriteBatch;
        std::shared_ptr<ResourceManager> m_resources;

    private:
        void handleEvents();

        bool m_isRunning;
    };
}