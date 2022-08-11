#pragma once

#include "Transformable.hpp"

class Camera2D : public Transformable
{
public:
    explicit Camera2D(const glm::ivec2& windowSize);
    Camera2D(const glm::ivec2& windowSize, float scale, float origin);
    ~Camera2D() = default;

    Camera2D() = delete;
    Camera2D(Camera2D&&) = delete;
    Camera2D(const Camera2D&) = delete;

    void setWindowSize(int width, int height);
    void setWindowSize(const glm::ivec2& windowSize);

protected:
    using Transformable::setSize;
    glm::mat4 getIdentity() override;

private:
    glm::mat4 m_orthoMatrix;
};
