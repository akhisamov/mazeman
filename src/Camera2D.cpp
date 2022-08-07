#include "Camera2D.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{
    constexpr glm::vec3 front(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 up(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 right(1.0f, 0.0f, 0.0f);
}

Camera2D::Camera2D(const glm::ivec2& windowSize)
    : m_windowSize(windowSize)
    , m_origin(0.5f)
    , m_position(0.0f, 0.0f, 1.0f)
    , m_scale(1.0f)
    , m_cameraMatrix(1.0f)
    , m_orthoMatrix(glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y))
{
    updateMatrix();
}

Camera2D::Camera2D(const glm::ivec2& windowSize, float scale, float origin)
    : m_windowSize(windowSize)
    , m_scale(scale)
    , m_origin(origin)
    , m_position(0.0f, 0.0f, 1.0f)
    , m_cameraMatrix(1.0f)
    , m_orthoMatrix(glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y))
{
    updateMatrix();
}

void Camera2D::moveX(float velocity)
{
    m_position += right * velocity;
    updateMatrix();
}

void Camera2D::moveY(float velocity)
{
    m_position += up * velocity;
    updateMatrix();
}

void Camera2D::setScale(float scale)
{
    m_scale = scale;
    updateMatrix();
}

void Camera2D::setOrigin(float value) { setOrigin(glm::vec2(value)); }

void Camera2D::setOrigin(const glm::vec2& origin)
{
    m_origin = origin;
    updateMatrix();
}

void Camera2D::setWindowSize(int width, int height) { setWindowSize(glm::ivec2(width, height)); }

void Camera2D::setWindowSize(const glm::ivec2& windowSize)
{
    m_windowSize = windowSize;
    m_orthoMatrix = glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y);
    updateMatrix();
}

void Camera2D::updateMatrix()
{
    const glm::vec3 positionTranslate(-m_position.x, -m_position.y, 0.0f);
    m_cameraMatrix = glm::translate(m_orthoMatrix, positionTranslate);

    const glm::vec3 originTranslate(m_origin.x * m_windowSize.x, m_origin.y * m_windowSize.y, 0.0f);
    m_cameraMatrix = glm::translate(m_cameraMatrix, originTranslate);

    m_cameraMatrix = glm::scale(m_cameraMatrix, glm::vec3(m_scale, m_scale, 0.0f));
}
