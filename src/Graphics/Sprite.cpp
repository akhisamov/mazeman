#include "Sprite.hpp"

#include <stdexcept>

std::shared_ptr<Sprite> Sprite::create(const std::shared_ptr<Texture2D>& texture)
{
    if (texture == nullptr)
    {
        throw std::runtime_error("Failed to create Sprite: texture is empty");
    }
    return std::make_shared<Sprite>(Sprite::Data { texture });
}

Sprite::Sprite(const Data& data)
    : Transformable(0.0f, glm::vec2(data.texture->getSize()), glm::vec2(0.5f), glm::vec2(1.0f), glm::vec2(0.0f))
    , m_texture(data.texture)
    , m_color(1.0f)
{
}

void Sprite::setColor(const glm::vec3& color) { m_color = color; }

void Sprite::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_color = glm::vec3(r / UINT8_MAX, g / UINT8_MAX, b / UINT8_MAX);
}

