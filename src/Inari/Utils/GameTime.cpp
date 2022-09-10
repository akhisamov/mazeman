#include "GameTime.hpp"

#include <SDL_timer.h>

namespace inari
{
    GameTime::GameTime()
        : m_startTicks(SDL_GetTicks())
    {
    }

    float GameTime::getTotalMs() const { return (SDL_GetTicks() - m_startTicks) / 1000.0f; }
}
