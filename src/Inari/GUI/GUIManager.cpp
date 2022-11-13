#include "GUIManager.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "Inari/Graphics/Window.hpp"

using namespace inari;

GUIManager::GUIManager(const std::shared_ptr<Window>& window) {
    assert(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    window->setupGui();
}

GUIManager::~GUIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUIManager::removeWindow(const std::string_view& name) {
    m_windows.erase(name.data());
}

void GUIManager::draw() {
    if (m_windows.empty()) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
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
    ImGui_ImplSDL2_ProcessEvent(&event);
}
