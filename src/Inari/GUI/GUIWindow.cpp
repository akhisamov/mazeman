#include "GUIWindow.hpp"

#include <imgui.h>

using namespace inari;

void GUIWindow::drawDemo(bool& isOpen) {
    ImGui::ShowDemoWindow(&isOpen);
}

void GUIWindow::begin(const std::string_view& title) {
    ImGui::Begin(title.data());
}

void GUIWindow::begin(const std::string_view& title, bool& isOpen) {
    ImGui::Begin(title.data(), &isOpen);
}

void GUIWindow::end() {
    ImGui::End();
}
