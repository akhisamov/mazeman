#pragma once

#include <cstdint>

namespace inari
{
    class GameTime
    {
        friend class IGame;
    public:
        GameTime();

        float getTotalMs() const;
        float getElapsedMs() const;

    protected:
        void begin();
        void end();

    private:
        uint32_t m_startTicks;
        uint32_t m_currentTicks;
        uint32_t m_lastUpdateTicks;
    };
}
