#pragma once

#include <string>

namespace inari {
class GUIWindow {
   public:
    virtual void draw() = 0;

   protected:
    // todo window flags -> ImGuiWindowFlags

    void drawDemo(bool& isOpen);

    void begin(const std::string_view& title);
    void begin(const std::string_view& title, bool& isOpen);
    void end();
};
}  // namespace inari
