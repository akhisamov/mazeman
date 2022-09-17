#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>


namespace inari {
class Camera2D {
   public:
    explicit Camera2D(const glm::ivec2& windowSize);
    Camera2D(const glm::ivec2& windowSize, float scale);
    ~Camera2D() = default;

    Camera2D() = delete;
    Camera2D(Camera2D&&) = delete;
    Camera2D(const Camera2D&) = delete;

    void setWindowSize(int width, int height);
    void setWindowSize(const glm::ivec2& windowSize);

    void setPositionX(float value);
    void setPositionY(float value);
    void setPosition(float x, float y);
    void setPosition(const glm::vec2& position);

    void moveX(float value);
    void moveY(float value);
    void move(float x, float y);
    void move(const glm::vec2& shift);

    const glm::vec2& getScale() const { return m_scale; }
    void setScale(float scale);
    void setScale(float x, float y);
    void setScale(const glm::vec2& scale);

    const glm::mat4& getTransform();

   private:
    bool m_isDirty;

    glm::mat4 m_transformMatrix;

    glm::vec2 m_windowSize;
    glm::vec2 m_position;
    glm::vec2 m_scale;
};
}  // namespace inari
