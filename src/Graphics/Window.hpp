#pragma once

#include <memory>
#include <string_view>

class Window
{
public:
    explicit Window(const std::string_view& title, int width, int height);
    ~Window();

    void display();

    struct Size
    {
        int width = 0;
        int height = 0;
    };
    Size getWindowSize() const;

private:
    std::unique_ptr<struct WindowData> m_data;
};
