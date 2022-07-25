#include "Camera2D.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace
{
    constexpr glm::vec3 front(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 up(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 right(1.0f, 0.0f, 0.0f);
}

Camera2D::Camera2D()
    : m_windowSize(0.0f)
    , m_position(0.0f, 0.0f, 1.0f)
    , m_scale(1.0f)
    , m_cameraMatrix(1.0f)
    , m_orthoMatrix(1.0f)
{
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

void Camera2D::setWindowSize(int width, int height)
{
    m_windowSize = glm::vec2(static_cast<float>(width), static_cast<float>(height));
    m_orthoMatrix = glm::ortho(0.0f, m_windowSize.x, 0.0f, m_windowSize.y);
    updateMatrix();
}

void Camera2D::updateMatrix()
{
    glm::vec3 translate(-m_position.x, -m_position.y, 0.0f);
    m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

    m_cameraMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale, m_scale, 0.0f)) * m_cameraMatrix;
}
