#include "InputManager.hpp"

#include <SDL_events.h>

namespace inari {

InputManager::KeyState InputManager::getState(SDL_Keycode keyCode) const {
    auto it = m_states.find(keyCode);
    if (it != m_states.end()) {
        return it->second;
    }
    return KeyState::UP;
}

bool InputManager::isKeyPressed(SDL_Keycode keyCode) const {
    const KeyState state = getState(keyCode);
    return state == KeyState::PRESSED;
}

bool InputManager::isKeyDown(SDL_Keycode keyCode) const {
    const KeyState state = getState(keyCode);
    return state == KeyState::DOWN || state == KeyState::PRESSED;
}

bool InputManager::isKeyReleased(SDL_Keycode keyCode) const {
    const KeyState state = getState(keyCode);
    return state == KeyState::RELEASED;
}

bool InputManager::isKeyUp(SDL_Keycode keyCode) const {
    const KeyState state = getState(keyCode);
    return state == KeyState::UP || state == KeyState::RELEASED;
}

void InputManager::prepareHandling() {
    for (auto& [keyCode, state] : m_states) {
        if (state == KeyState::PRESSED) {
            state = KeyState::DOWN;
        } else if (state == KeyState::RELEASED) {
            state = KeyState::UP;
        }
    }
}

void InputManager::handleEvent(const SDL_KeyboardEvent& event) {
    auto state = KeyState::UP;
    if (event.state == SDL_RELEASED) {
        state = KeyState::RELEASED;
    } else if (event.state == SDL_PRESSED) {
        state = KeyState::PRESSED;
    }
    const SDL_Keycode code = event.keysym.sym;

    m_states[code] = state;
}

}  // namespace inari