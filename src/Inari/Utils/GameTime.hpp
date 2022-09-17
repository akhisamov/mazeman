#pragma once

#include <cstdint>

namespace inari {
class GameTime {
   public:
    GameTime();

    float getTotalMs() const;

   private:
    uint32_t m_startTicks;
};
}  // namespace inari
