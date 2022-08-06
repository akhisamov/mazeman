#pragma once

#include <memory>

#include "Utils/GameTime.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    void init();

    void loadResource();
    void unloadResource();

    void handleEvents();
    void update(const GameTime& gameTime);
    void draw();

    bool m_isRunning;

    std::unique_ptr<struct GameData> m_data;

    std::shared_ptr<class ResourceManager> m_resources;
    std::unique_ptr<class Window> m_window;
    std::unique_ptr<class SpriteRenderer> m_renderer;
};
