#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string_view>

namespace inari {
class Window {
    friend class IGame;

   public:
    static std::shared_ptr<Window> create(const std::string_view& title,
                                          int width,
                                          int height);

    explicit Window(std::unique_ptr<struct WindowData>&& data);
    ~Window();

    void clear(const glm::vec4& color);
    void clear(const glm::vec3& color);
    void display();

    void setWindowSize(const glm::ivec2& size);
    glm::ivec2 getWindowSize() const;

    void setTitle(const std::string_view& title);

    void setFrameLimit(int screenFps);

   protected:
    void begin();
    void end();

   private:
    std::unique_ptr<struct WindowData> m_data;

    std::unique_ptr<int> m_frameLimit;
};
}  // namespace inari
