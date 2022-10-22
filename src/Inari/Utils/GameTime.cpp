#include "GameTime.hpp"

#include <SDL_timer.h>

namespace inari {
GameTime::GameTime()
    : m_currentTicks(SDL_GetTicks()),
      m_startTicks(m_currentTicks),
      m_lastTicks(m_currentTicks) {}

float GameTime::getTotalMs() const {
    return static_cast<float>(m_currentTicks - m_startTicks) / 1000.0f;
}

float GameTime::getElapsedGameTime() const {
    return static_cast<float>(m_currentTicks - m_lastTicks) / 1000.0f;
}

void GameTime::reset() {
    m_lastTicks = m_currentTicks;
    m_currentTicks = SDL_GetTicks();
}

}  // namespace inari
