#pragma once

#include <glm/mat4x4.hpp>

class Camera2D
{
public:
    Camera2D();

    const glm::mat4& getView() const { return m_view; }

    void moveX(float velocity);
    void moveY(float velocity);

private:
    void updateView();

    glm::vec3 m_position;
    glm::mat4 m_view;
};
