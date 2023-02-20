#pragma once

#include <cstdint>

namespace inari {
    class GameTime {
        friend class BaseGame;

    public:
        GameTime();

        float getTotalMs() const;
        float getElapsedTime() const;

        static uint32_t getCurrentTicks();

    protected:
        void reset();

    private:
        uint32_t m_currentTicks;

        uint32_t m_startTicks;
        uint32_t m_lastTicks;
    };
} // namespace inari
