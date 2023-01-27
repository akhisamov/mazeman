#include "Camera2D.hpp"

#include <glm/ext/matrix_clip_space.hpp>

namespace inari {
    Camera2D::Camera2D(const glm::ivec2& windowSize)
        : m_isDirty(true)
        , m_transformMatrix(1.0f)
        , m_windowSize(windowSize)
        , m_position(0.0f)
        , m_scale(1.0f)
    {
    }

    Camera2D::Camera2D(const glm::ivec2& windowSize, float scale)
        : m_isDirty(true)
        , m_transformMatrix(1.0f)
        , m_windowSize(windowSize)
        , m_position(0.0f)
        , m_scale(scale)
    {
    }

    void Camera2D::setWindowSize(const glm::vec2& windowSize)
    {
        m_windowSize = windowSize;
        m_isDirty = true;
    }

    void Camera2D::setPosition(const glm::vec2& position)
    {
        m_position = position;
        m_isDirty = true;
    }

    void Camera2D::move(const glm::vec2& shift)
    {
        m_position = m_position + shift;
        m_isDirty = true;
    }

    void Camera2D::setScale(const glm::vec2& scale)
    {
        m_scale = scale;
        m_isDirty = true;
    }

    const glm::mat4& Camera2D::getTransform()
    {
        if (m_isDirty) {
            m_transformMatrix = glm::ortho(m_position.x, (m_windowSize.x * m_scale.x) + m_position.x,
                                           (m_windowSize.y * m_scale.y) + m_position.y, m_position.y);
        }

        return m_transformMatrix;
    }
} // namespace inari
