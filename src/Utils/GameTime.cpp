#include "GameTime.hpp"

#include <SDL_timer.h>

GameTime::GameTime()
    : m_startTicks(SDL_GetTicks())
    , m_currentTicks(0)
    , m_lastUpdateTicks(SDL_GetTicks())
{
}

float GameTime::getTotalMs() const { return (SDL_GetTicks() - m_startTicks) / 1000.0f; }
float GameTime::getElapsedMs() const { return (m_currentTicks - m_lastUpdateTicks) / 1000.0f; }

void GameTime::beginUpdate() { m_currentTicks = SDL_GetTicks(); }
void GameTime::endUpdate() { m_lastUpdateTicks = m_currentTicks; }
