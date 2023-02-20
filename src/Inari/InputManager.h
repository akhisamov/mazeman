#pragma once

#include <SDL_events.h>
#include <SDL_keycode.h>

#include <map>

#include "IService.h"

namespace inari {
    class InputManager final : public IService {
        friend class BaseGame;

    public:
        // Key has been pressed
        bool isKeyPressed(SDL_Keycode keyCode) const;
        // Key is being pressed
        bool isKeyDown(SDL_Keycode keyCode) const;
        // Key has been released
        bool isKeyReleased(SDL_Keycode keyCode) const;
        // Key is not being pressed
        bool isKeyUp(SDL_Keycode keyCode) const;

    protected:
        struct Token { };
        static std::shared_ptr<InputManager> create();

        void prepareHandling();
        void handleEvent(const SDL_KeyboardEvent& event);

    public:
        explicit InputManager(Token);
        ~InputManager() override;

        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(InputManager&&) = delete;

    private:
        enum class KeyState { RELEASED = 0, PRESSED = 1, DOWN = 2, UP = 3 };

        KeyState getState(SDL_Keycode keyCode) const;

        std::map<SDL_Keycode, KeyState> m_states;
    };
} // namespace inari