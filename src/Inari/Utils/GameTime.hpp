#pragma once

#include <cstdint>

namespace inari {
class GameTime {
    friend class IGame;

   public:
    GameTime();

    float getTotalMs() const;
    float getElapsedGameTime() const;

   protected:
    void reset();

   private:
    uint32_t m_currentTicks;

    uint32_t m_startTicks;
    uint32_t m_lastTicks;
};
}  // namespace inari
