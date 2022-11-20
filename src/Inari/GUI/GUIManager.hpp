#pragma once

#include <SDL_events.h>

#include <memory>
#include <unordered_map>

#include "GUIWindow.hpp"

namespace inari {
class Window;
using GUIWindowPtr = std::shared_ptr<GUIWindow>;

class GUIManager final {
    friend class IGame;

   public:
    GUIManager(std::shared_ptr<Window> window);
    ~GUIManager();

    GUIManager() = delete;
    GUIManager(GUIManager&&) = delete;
    GUIManager(const GUIManager&) = delete;

    template <class W, typename... Args>
    void addWindow(const std::string_view& name, Args... args) {
        static_assert(std::is_base_of_v<GUIWindow, W>, "Is not GUI Window");
        m_windows[name.data()] = std::make_shared<W>(args...);
    }
    void removeWindow(const std::string_view& name);

    void draw();

   protected:
    void handleEvent(const SDL_Event& event);

   private:
    std::shared_ptr<Window> m_window;

    std::unordered_map<std::string, GUIWindowPtr> m_windows;

    const uint64_t m_frequency;
};
}  // namespace inari
