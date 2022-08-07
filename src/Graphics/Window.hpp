#pragma once

#include <glm/ext/vector_int2.hpp>

#include <memory>
#include <string_view>

class Window
{
public:
    explicit Window(const std::string_view& title, int width, int height);
    ~Window();

    void display();

    glm::ivec2 getWindowSize() const;

    void setTitle(const std::string_view& title);

private:
    std::unique_ptr<struct WindowData> m_data;
};
