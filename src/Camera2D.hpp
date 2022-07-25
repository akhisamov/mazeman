#pragma once

#include <glm/mat4x4.hpp>

class Camera2D
{
public:
    Camera2D();

    void moveX(float velocity);
    void moveY(float velocity);

    float getScale() const { return m_scale; }
    void setScale(float scale);

    void setWindowSize(int width, int height);

    const glm::mat4& getMatrix() const { return m_cameraMatrix; }

private:
    void updateMatrix();

    float m_scale;

    glm::vec2 m_windowSize;

    glm::vec3 m_position;

    glm::mat4 m_cameraMatrix;
    glm::mat4 m_orthoMatrix;
};
