#pragma once

#include <glm/mat4x4.hpp>

#include <cmath>

class Transformable
{
public:
    virtual float getRadians() const { return m_radians; }
    virtual void setRadians(float radians);
    virtual float getDegrees() const;
    virtual void setDegrees(float degrees);

    virtual const glm::vec2& getSize() const { return m_size; }
    virtual void setSize(float width, float height) { setSize(glm::vec2(width, height)); }
    virtual void setSize(const glm::vec2& size);

    virtual const glm::vec2& getOrigin() const { return m_origin; }
    virtual void setOrigin(float x, float y) { setOrigin(glm::vec2(x, y)); }
    virtual void setOrigin(const glm::vec2& origin);

    virtual const glm::vec2& getScale() const { return m_scale; }
    virtual void setScale(float scale) { setScale(glm::vec2(scale)); }
    virtual void setScale(const glm::vec2& scale);

    virtual const glm::vec2& getPosition() const { return m_position; }
    virtual void setPosition(int x, int y) { setPosition(glm::vec2(x, y)); }
    virtual void setPosition(const glm::vec2& position);
    virtual void move(float offsetX, float offsetY) { move(glm::vec2(offsetX, offsetY)); }
    virtual void move(const glm::vec2& offset);

    const glm::mat4& getTransformation();

protected:
    Transformable(float radians, const glm::vec2& size, const glm::vec2& origin, const glm::vec2& scale,
                  const glm::vec2& position);
    ~Transformable() = default;

    virtual glm::mat4 getIdentity() { return glm::mat4(1.0f); }

    void setDirty();

private:
    bool m_isDirty;

    float m_radians;

    glm::vec2 m_size;
    glm::vec2 m_origin;
    glm::vec2 m_scale;
    glm::vec2 m_position;

    glm::mat4 m_transformation;
};
