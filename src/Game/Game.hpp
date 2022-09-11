#pragma once

#include "Inari/IGame.hpp"

namespace inari
{
    class EntityRegistry;
    class SystemRegistry;
    class Camera2D;
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

    void update(float dt) override;
    void draw(float dt) override;

    std::shared_ptr<inari::EntityRegistry> m_entityRegistry;
    std::unique_ptr<inari::SystemRegistry> m_systemRegistry;

    std::unique_ptr<inari::Camera2D> m_camera;

    int trackIdx = 0;
};
