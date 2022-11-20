#include "GameTime.hpp"

#include <SDL_timer.h>

namespace inari {
GameTime::GameTime()
    : m_currentTicks(getCurrentTicks()),
      m_startTicks(m_currentTicks),
      m_lastTicks(m_currentTicks) {}

float GameTime::getTotalMs() const {
    return static_cast<float>(m_currentTicks - m_startTicks) / 1000.0f;
}

float GameTime::getElapsedTime() const {
    return static_cast<float>(m_currentTicks - m_lastTicks) / 1000.0f;
}

uint32_t GameTime::getCurrentTicks() {
    return SDL_GetTicks();
}

uint64_t GameTime::getFrequency() {
    return SDL_GetPerformanceFrequency();
}

uint64_t GameTime::getCurrentCounter() {
    return SDL_GetPerformanceCounter();
}

void GameTime::reset() {
    m_lastTicks = m_currentTicks;
    m_currentTicks = getCurrentTicks();
}

}  // namespace inari
