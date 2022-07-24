#pragma once

#include <glm/mat4x4.hpp>

#include "Resources/Texture2D.hpp"

class Texture2D;

class Sprite final
{
    friend class SpriteRenderer;

protected:
    struct Data
    {
        const std::shared_ptr<Texture2D>& texture;
    };

public:
    static std::shared_ptr<Sprite> create(const std::shared_ptr<Texture2D>& texture);

    explicit Sprite(const Data& data);

    [[nodiscard]] const std::shared_ptr<Texture2D>& getTexture() const { return m_texture; }

    [[nodiscard]] const glm::vec2& getPosition() const { return m_position; }
    void setPosition(const glm::vec2& position);
    void setPosition(float x, float y);

    [[nodiscard]] const glm::vec2& getOrigin() const { return m_origin; }
    void setOrigin(const glm::vec2& origin);
    void setOrigin(float x, float y);

    [[nodiscard]] float getDegrees() const { return m_degrees; }
    void setDegrees(float degrees);

    [[nodiscard]] const glm::vec3& getColor() const { return m_color; }
    void setColor(const glm::vec3& color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

protected:
    const glm::mat4& getModel() const { return m_model; }

private:
    void updateModel();

    const std::shared_ptr<Texture2D>& m_texture;

    glm::vec2 m_position;
    glm::vec2 m_origin;
    float m_degrees;
    glm::vec3 m_color;

    glm::mat4 m_model;
};
