#include "Camera2D.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace
{
    constexpr glm::vec3 front(0.0f, 0.0f, -1.0f);
    constexpr glm::vec3 up(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 right(1.0f, 0.0f, 0.0f);
}

Camera2D::Camera2D()
    : m_position(0.0f, 0.0f, 1.0f)
    , m_view(1.0f)
{
    updateView();
}

void Camera2D::moveX(float velocity)
{
    m_position += right * velocity;
    updateView();
}

void Camera2D::moveY(float velocity)
{
    m_position += up * velocity;
    updateView();
}

void Camera2D::updateView() { m_view = glm::lookAt(m_position, m_position + front, up); }
