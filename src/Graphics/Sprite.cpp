#include "Sprite.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <stdexcept>

std::shared_ptr<Sprite> Sprite::create(const std::shared_ptr<Texture2D>& texture)
{
    if (texture == nullptr)
    {
        throw std::runtime_error("Failed to create Sprite: texture is empty");
    }
    return std::shared_ptr<Sprite>(new Sprite(texture));
}

Sprite::Sprite(const std::shared_ptr<Texture2D>& texture)
    : m_texture(texture)
    , m_position(0.0f)
    , m_origin(0.5f)
    , m_degrees(0.0f)
    , m_color(1.0f)
    , m_model(1.0f)
{
}

void Sprite::setPosition(const glm::vec2& position)
{
    m_position = position;
    updateModel();
}

void Sprite::setPosition(float x, float y) { m_position = glm::vec2(x, y); }

void Sprite::setOrigin(const glm::vec2& origin)
{
    m_origin = origin;
    updateModel();
}

void Sprite::setOrigin(float x, float y)
{
    m_origin = glm::vec2(x, y);
    updateModel();
}

void Sprite::setDegrees(float degrees)
{
    m_degrees = degrees;
    updateModel();
}

void Sprite::setColor(const glm::vec3& color)
{
    m_color = color;
    updateModel();
}

void Sprite::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_color = glm::vec3(r / UINT8_MAX, g / UINT8_MAX, b / UINT8_MAX);
    updateModel();
}

void Sprite::updateModel()
{
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, glm::vec3(m_position, 0.0f));

    m_model = glm::rotate(m_model, glm::radians(m_degrees), glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::vec2& size = m_texture->getSize();
    m_model = glm::translate(m_model, glm::vec3(-m_origin.x * size.x, -m_origin.y * size.y, 0.0f));

    m_model = glm::scale(m_model, glm::vec3(size, 1.0f));
}
