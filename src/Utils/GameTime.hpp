#pragma once

#include <cstdint>

class GameTime
{
    friend class Game;

public:
    GameTime();

    float getTotalMs() const;
    float getElapsedMs() const;

protected:
    void beginUpdate();
    void endUpdate();

private:
    uint32_t m_startTicks;
    uint32_t m_currentTicks;
    uint32_t m_lastUpdateTicks;
};
