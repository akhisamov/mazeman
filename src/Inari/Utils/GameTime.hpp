#pragma once

#include <cstdint>

namespace inari {
class GameTime {
    friend class IGame;

   public:
    GameTime();

    float getTotalMs() const;
    float getElapsedTime() const;

    static uint32_t getCurrentTicks();

    static uint64_t getFrequency();
    static uint64_t getCurrentCounter();

   protected:
    void reset();

   private:
    uint32_t m_currentTicks;

    uint32_t m_startTicks;
    uint32_t m_lastTicks;
};
}  // namespace inari
