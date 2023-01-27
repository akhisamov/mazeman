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

        const glm::vec2& getWindowSize() const { return m_windowSize; }
        void setWindowSize(const glm::vec2& windowSize);

        const glm::vec2& getPosition() const { return m_position; }
        void setPosition(const glm::vec2& position);
        void move(const glm::vec2& shift);

        const glm::vec2& getScale() const { return m_scale; }
        void setScale(const glm::vec2& scale);

        const glm::mat4& getTransform();

    private:
        bool m_isDirty;

        glm::mat4 m_transformMatrix;

        glm::vec2 m_windowSize;
        glm::vec2 m_position;
        glm::vec2 m_scale;
    };
} // namespace inari
