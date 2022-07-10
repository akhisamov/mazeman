#pragma once

#include <memory>

class Game
{
public:
    static std::unique_ptr<Game> create();
    ~Game();

    void init();

    void loadResource();
    void unloadResource();

    void handleEvents();
    void draw();

    [[nodiscard]] bool isRunning() const { return m_isRunning; }

private:
    Game();

    void initRenderer();

    bool m_isRunning;

    std::unique_ptr<struct GameData> m_data;

    std::shared_ptr<class ResourceManager> m_resources;
    std::unique_ptr<class Window> m_window;
    std::unique_ptr<class SpriteRenderer> m_renderer;
};
