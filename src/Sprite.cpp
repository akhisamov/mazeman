#include "Sprite.hpp"

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
{
}

void Sprite::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_color = glm::vec3(r / UINT8_MAX, g / UINT8_MAX, b / UINT8_MAX);
}
