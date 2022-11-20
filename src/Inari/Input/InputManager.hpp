#pragma once

#include <SDL_events.h>
#include <SDL_keycode.h>

#include <map>

namespace inari {

class InputManager {
    friend class IGame;
    friend class GUIManager;

   public:
    InputManager() = default;
    ~InputManager() = default;

    // Key has been pressed
    bool isKeyPressed(SDL_Keycode keyCode) const;
    // Key is being pressed
    bool isKeyDown(SDL_Keycode keyCode) const;
    // Key has been released
    bool isKeyReleased(SDL_Keycode keyCode) const;
    // Key is not being pressed
    bool isKeyUp(SDL_Keycode keyCode) const;

   protected:
    void prepareHandling();
    void handleEvent(const SDL_KeyboardEvent& event);

    static int keycodeToGuiKey(SDL_Keycode keyCode);

   private:
    enum class KeyState { RELEASED = 0, PRESSED = 1, DOWN = 2, UP = 3 };

    KeyState getState(SDL_Keycode keyCode) const;

    std::map<SDL_Keycode, KeyState> m_states;
};

}  // namespace inari
