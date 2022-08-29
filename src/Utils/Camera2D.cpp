#include "Camera2D.hpp"

#include "glm/ext/matrix_clip_space.hpp"

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

void Camera2D::setWindowSize(int width, int height) { setWindowSize(glm::ivec2(width, height)); }
void Camera2D::setWindowSize(const glm::ivec2& windowSize)
{
    m_transformMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y));
    m_isDirty = true;
}

void Camera2D::setPositionX(float value) { setPosition(glm::vec2(value, m_position.y)); }
void Camera2D::setPositionY(float value) { setPosition(glm::vec2(m_position.x, value)); }
void Camera2D::setPosition(float x, float y) { setPosition(glm::vec2(x, y)); }
void Camera2D::setPosition(const glm::vec2& position)
{
    m_position = position;
    m_isDirty = true;
}

void Camera2D::moveX(float value) { move(glm::vec2(value, 0)); }
void Camera2D::moveY(float value) { move(glm::vec2(0, value)); }
void Camera2D::move(float x, float y) { move(glm::vec2(x, y)); }
void Camera2D::move(const glm::vec2& shift)
{
    m_position.x += shift.x;
    m_position.y += shift.y;
    m_isDirty = true;
}

void Camera2D::setScale(float scale) { setScale(glm::vec2(scale)); }
void Camera2D::setScale(float x, float y) { setScale(glm::vec2(x, y)); }
void Camera2D::setScale(const glm::vec2& scale)
{
    m_scale = scale;
    m_isDirty = true;
}

const glm::mat4& Camera2D::getTransform()
{
    if (m_isDirty)
    {
        m_transformMatrix = glm::ortho(m_position.x, (m_windowSize.x * m_scale.x) + m_position.x, m_position.y,
                                       (m_windowSize.y * m_scale.y) + m_position.y);
    }

    return m_transformMatrix;
}
