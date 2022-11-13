#pragma once

#include "Inari/GUI/GUIWindow.hpp"

class DemoWindow : public inari::GUIWindow {
   public:
    void draw() override { drawDemo(m_show); }

   private:
    bool m_show = true;
};
