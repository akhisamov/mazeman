#pragma once

#include <cstdint>

namespace inari
{
    class GameTime
    {
    public:
        GameTime();

        float getTotalMs() const;
        float getElapsedMs() const;

        void beginUpdate();
        void endUpdate();

    private:
        uint32_t m_startTicks;
        uint32_t m_currentTicks;
        uint32_t m_lastUpdateTicks;
    };
}
