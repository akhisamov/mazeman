#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Resources/Texture2D.hpp"

class Texture2D;

class Sprite
{
public:
    static std::shared_ptr<Sprite> create(const std::shared_ptr<Texture2D>& texture);

    [[nodiscard]] const std::shared_ptr<Texture2D>& getTexture() const { return m_texture; }

    [[nodiscard]] const glm::vec2& getPosition() const { return m_position; }
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = glm::vec2(x, y); }

    [[nodiscard]] const glm::vec2& getOrigin() const { return m_origin; }
    void setOrigin(const glm::vec2& origin) { m_origin = origin; }
    void setOrigin(float x, float y) { m_origin = glm::vec2(x, y); }

    [[nodiscard]] float getDegrees() const { return m_degrees; }
    void setDegrees(float degrees) { m_degrees = degrees; }

    [[nodiscard]] const glm::vec3& getColor() const { return m_color; }
    void setColor(const glm::vec3& color) { m_color = color; }
    void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    explicit Sprite(const std::shared_ptr<Texture2D>& texture);

    const std::shared_ptr<Texture2D>& m_texture;

    glm::vec2 m_position;
    glm::vec2 m_origin;
    float m_degrees;
    glm::vec3 m_color;
};
