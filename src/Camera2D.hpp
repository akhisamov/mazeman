#pragma once

#include <glm/mat4x4.hpp>

class Camera2D
{
public:
    explicit Camera2D(const glm::ivec2& windowSize);
    Camera2D(const glm::ivec2& windowSize, float scale, float origin);

    void moveX(float velocity);
    void moveY(float velocity);

    float getScale() const { return m_scale; }
    void setScale(float scale);

    glm::vec2 getOrigin() const { return m_origin; }
    void setOrigin(float value);
    void setOrigin(const glm::vec2& origin);

    void setWindowSize(int width, int height);
    void setWindowSize(const glm::ivec2& windowSize);

    const glm::mat4& getMatrix() const { return m_cameraMatrix; }

private:
    void updateMatrix();

    float m_scale;

    glm::vec2 m_windowSize;
    glm::vec2 m_origin;

    glm::vec3 m_position;

    glm::mat4 m_cameraMatrix;
    glm::mat4 m_orthoMatrix;
};
