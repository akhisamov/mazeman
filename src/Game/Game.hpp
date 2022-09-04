#pragma once

#include "Inari/IGame.hpp"

namespace inari
{
    class GameTime;
}

class Game final : public inari::IGame
{
public:
    Game();
    ~Game();

private:
    bool init() override;

    void loadResources() override;
    void unloadResources() override;

    void handleWindowResized(const glm::ivec2& size) override;

    void update(const inari::GameTime& gameTime) override;
    void draw() override;

    std::unique_ptr<struct GameData> m_data;
};
