#include "GUIManager.hpp"

#include <SDL.h>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "Inari/Graphics/Window.hpp"

#include "Inari/Input/InputManager.hpp"

#include "Inari/Utils/GameTime.hpp"

#include <algorithm>
#include <array>
#include <map>
#include <string>

using namespace inari;

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && \
    !(defined(__APPLE__) && TARGET_OS_IOS) && !defined(__amigaos4__)
#define SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE 1
#else
#define SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE 0
#endif

namespace {
ImVec2 vec2cast(const glm::ivec2& from) {
    return ImVec2(static_cast<float>(from.x), static_cast<float>(from.y));
}

glm::vec2 simplifyWheelPosition(const SDL_MouseWheelEvent& wheel) {
    glm::vec2 wheelPos(wheel.x, wheel.y);
    glm::normalize(wheelPos);
    if (wheelPos.x != 0.0f) {
        wheelPos.x =
            wheelPos.x > 0.0f ? std::ceil(wheelPos.x) : std::floor(wheelPos.x);
    }
    if (wheelPos.y != 0.0f) {
        wheelPos.y =
            wheelPos.y > 0.0f ? std::ceil(wheelPos.y) : std::floor(wheelPos.y);
    }
    return wheelPos;
}

constexpr float defaultDeltaTime = 1.0f / 60.0f;

struct ImGuiImplSDL2Data {
    uint64_t time;
    int mouseButtonsDown;
    std::map<ImGuiMouseCursor, SDL_Cursor*> mouseCursors;
    int pendingMouseLeaveFrame;
    char* clipboardTextData;
    bool mouseCanUseGlobalState;

    ImGuiImplSDL2Data()
        : time(0),
          mouseButtonsDown(0),
          pendingMouseLeaveFrame(0),
          clipboardTextData(nullptr),
          mouseCanUseGlobalState(false) {
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
        const char* sdlBackend = SDL_GetCurrentVideoDriver();
        constexpr std::array<std::string_view, 5> globalMouseWhitelist = {
            "windows", "cocoa", "x11", "DIVE", "VMAN"};
        mouseCanUseGlobalState =
            std::find(globalMouseWhitelist.begin(), globalMouseWhitelist.end(),
                      sdlBackend) != globalMouseWhitelist.end();
#endif

        mouseCursors = {{ImGuiMouseCursor_Arrow,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)},
                        {ImGuiMouseCursor_TextInput,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)},
                        {ImGuiMouseCursor_ResizeAll,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL)},
                        {ImGuiMouseCursor_ResizeNS,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS)},
                        {ImGuiMouseCursor_ResizeEW,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE)},
                        {ImGuiMouseCursor_ResizeNESW,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW)},
                        {ImGuiMouseCursor_ResizeNWSE,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE)},
                        {ImGuiMouseCursor_Hand,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)},
                        {ImGuiMouseCursor_NotAllowed,
                         SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO)}};

        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
    }

    ~ImGuiImplSDL2Data() {
        if (clipboardTextData) {
            SDL_free(clipboardTextData);
        }
        for (auto& it : mouseCursors) {
            SDL_FreeCursor(it.second);
        }
    }

    SDL_Cursor* getCursor(ImGuiMouseCursor cursor) {
        auto it = mouseCursors.find(cursor);
        if (it == mouseCursors.end()) {
            it = mouseCursors.find(ImGuiMouseCursor_Arrow);
        }
        return it != mouseCursors.end() ? it->second : nullptr;
    }
};
}  // namespace

GUIManager::GUIManager(std::shared_ptr<Window> window)
    : m_window(std::move(window)), m_frequency(GameTime::getFrequency()) {
    assert(m_window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // SDL2 Init -->
    ImGuiIO& io = ImGui::GetIO();
    assert(io.BackendPlatformUserData == nullptr);
    io.BackendPlatformUserData =
        reinterpret_cast<void*>(new ImGuiImplSDL2Data());
    io.BackendPlatformName = "imguiImplSDL";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.SetClipboardTextFn = [](void*, const char* text) {
        SDL_SetClipboardText(text);
    };
    io.GetClipboardTextFn = [](void*) -> const char* {
        ImGuiIO& io = ImGui::GetIO();
        auto* bd =
            reinterpret_cast<ImGuiImplSDL2Data*>(io.BackendPlatformUserData);
        if (bd == nullptr) {
            return "";
        }
        if (bd->clipboardTextData) {
            SDL_free(bd->clipboardTextData);
        }
        bd->clipboardTextData = SDL_GetClipboardText();
        return bd->clipboardTextData;
    };
    io.ClipboardUserData = nullptr;

#ifdef _WIN32
    HWND hwnd = m_window->getHWND();
    if (hwnd) {
        ImGui::GetMainViewport()->PlatformHandleRaw = (void*)hwnd;
    }
#endif
    // <-- SDL2 Init
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

GUIManager::~GUIManager() {
    ImGui_ImplOpenGL3_Shutdown();

    ImGuiIO& io = ImGui::GetIO();
    auto* bd = reinterpret_cast<ImGuiImplSDL2Data*>(io.BackendPlatformUserData);
    if (bd) {
        delete bd;
    }
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;

    ImGui::DestroyContext();
}

void GUIManager::removeWindow(const std::string_view& name) {
    m_windows.erase(name.data());
}

void GUIManager::draw() {
    if (m_windows.empty() || m_window == nullptr) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();

    // SDL2 NewFrame -->
    ImGuiIO& io = ImGui::GetIO();
    auto* bd = reinterpret_cast<ImGuiImplSDL2Data*>(io.BackendPlatformUserData);
    assert(bd != nullptr);
    const glm::ivec2 windowSize = m_window->getWindowSize();
    const glm::ivec2 drawableSize = m_window->getDrawableSize();
    io.DisplaySize = vec2cast(windowSize);
    if (windowSize.x > 0 && windowSize.y > 0) {
        io.DisplayFramebufferScale = vec2cast(drawableSize / windowSize);
    }

    const uint64_t currentTime = GameTime::getCurrentCounter();
    if (bd->time > 0) {
        io.DeltaTime = static_cast<float>(
            static_cast<double>(currentTime - bd->time) / m_frequency);
    } else {
        io.DeltaTime = defaultDeltaTime;
    }
    bd->time = currentTime;
    if (bd->pendingMouseLeaveFrame &&
        bd->pendingMouseLeaveFrame >= ImGui::GetFrameCount() &&
        bd->mouseButtonsDown == 0) {
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        bd->pendingMouseLeaveFrame = 0;
    }

    const ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imguiCursor == ImGuiMouseCursor_None) {
        SDL_ShowCursor(SDL_FALSE);
    } else {
        SDL_Cursor* cursor = bd->getCursor(imguiCursor);
        if (cursor) {
            SDL_SetCursor(cursor);
        }
        SDL_ShowCursor(cursor ? SDL_TRUE : SDL_FALSE);
    }
    // <-- SDL2 NewFrame

    ImGui::NewFrame();

    for (const auto& [name, window] : m_windows) {
        if (window) {
            window->draw();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::handleEvent(const SDL_Event& event) {
    ImGuiIO& io = ImGui::GetIO();
    auto* bd = reinterpret_cast<ImGuiImplSDL2Data*>(io.BackendPlatformUserData);
    assert(bd != nullptr);

    switch (event.type) {
        case SDL_MOUSEMOTION: {
            io.AddMousePosEvent(static_cast<float>(event.motion.x),
                                static_cast<float>(event.motion.y));
            break;
        }
        case SDL_MOUSEWHEEL: {
            const glm::vec2 wheelPos = simplifyWheelPosition(event.wheel);
            io.AddMouseWheelEvent(wheelPos.x, wheelPos.y);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int mouseButton = -1;
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseButton = ImGuiMouseButton_Left;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                mouseButton = ImGuiMouseButton_Right;
            } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                mouseButton = ImGuiMouseButton_Middle;
            } else if (event.button.button == SDL_BUTTON_X1) {
                mouseButton = SDL_BUTTON_X1 - 1;  // 3
            } else if (event.button.button == SDL_BUTTON_X2) {
                mouseButton = SDL_BUTTON_X2 - 1;  // 4
            }
            if (mouseButton > -1) {
                io.AddMouseButtonEvent(mouseButton,
                                       (event.type == SDL_MOUSEBUTTONDOWN));
                bd->mouseButtonsDown =
                    (event.type == SDL_MOUSEBUTTONDOWN)
                        ? (bd->mouseButtonsDown | (1 << mouseButton))
                        : (bd->mouseButtonsDown & ~(1 << mouseButton));
            }
            break;
        }
        case SDL_TEXTINPUT: {
            io.AddInputCharactersUTF8(event.text.text);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            const auto keyMods = static_cast<SDL_Keymod>(event.key.keysym.mod);
            io.AddKeyEvent(ImGuiKey_ModCtrl, (keyMods & KMOD_CTRL) != 0);
            io.AddKeyEvent(ImGuiKey_ModShift, (keyMods & KMOD_SHIFT) != 0);
            io.AddKeyEvent(ImGuiKey_ModAlt, (keyMods & KMOD_ALT) != 0);
            io.AddKeyEvent(ImGuiKey_ModSuper, (keyMods & KMOD_GUI) != 0);

            const ImGuiKey key =
                InputManager::keycodeToGuiKey(event.key.keysym.sym);
            io.AddKeyEvent(key, (event.type == SDL_KEYDOWN));
            io.SetKeyEventNativeData(key, event.key.keysym.sym,
                                     event.key.keysym.scancode,
                                     event.key.keysym.scancode);
            break;
        }
        case SDL_WINDOWEVENT: {
            const uint8_t windowEvent = event.window.event;
            if (windowEvent == SDL_WINDOWEVENT_ENTER)
                bd->pendingMouseLeaveFrame = 0;
            if (windowEvent == SDL_WINDOWEVENT_LEAVE)
                bd->pendingMouseLeaveFrame = ImGui::GetFrameCount() + 1;
            if (windowEvent == SDL_WINDOWEVENT_FOCUS_GAINED)
                io.AddFocusEvent(true);
            else if (windowEvent == SDL_WINDOWEVENT_FOCUS_LOST)
                io.AddFocusEvent(false);
            break;
        }
    }
}
