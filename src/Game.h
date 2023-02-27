#pragma once

#include <Inari/BaseGame.h>
#include <Inari/Common/VecTypes.h>

namespace inari {
    class EntityRegistry;
    class SystemRegistry;
    class Camera2D;
    class GameTime;
}

class Game final : public inari::BaseGame {
public:
    Game();
    ~Game();

private:
    bool init() override;

    void loadResources() override;
    void unloadResources() override;

    void handleWindowResized(const inari::Vec2i& size) override;

    void update(const inari::GameTime& gameTime) override;
    void draw(const inari::GameTime& gameTime) override;

    std::shared_ptr<inari::EntityRegistry> m_entityRegistry;
    std::unique_ptr<inari::SystemRegistry> m_systemRegistry;

    std::unique_ptr<inari::Camera2D> m_camera;
};
