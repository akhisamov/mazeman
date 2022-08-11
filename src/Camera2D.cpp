#include "Camera2D.hpp"

#include "glm/ext/matrix_clip_space.hpp"

Camera2D::Camera2D(const glm::ivec2& windowSize)
    : Transformable(0.0f, glm::vec2(1.0f), glm::vec2(0.5f), glm::vec2(1.0f), glm::vec2(0.0f))
    , m_orthoMatrix(glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y)))
{
}

Camera2D::Camera2D(const glm::ivec2& windowSize, float scale, float origin)
    : Transformable(0.0f, glm::vec2(1.0f), glm::vec2(origin), glm::vec2(scale), glm::vec2(0.0f))
    , m_orthoMatrix(glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y)))
{
}

void Camera2D::setWindowSize(int width, int height) { setWindowSize(glm::ivec2(width, height)); }

void Camera2D::setWindowSize(const glm::ivec2& windowSize)
{
    m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y));
    setDirty();
}

glm::mat4 Camera2D::getIdentity() { return m_orthoMatrix; }
