#pragma once

#include "Resources/Texture2D.hpp"

#include "Transformable.hpp"

class Texture2D;

class Sprite final : public Transformable
{
protected:
    struct Data
    {
        const std::shared_ptr<Texture2D>& texture;
    };

public:
    static std::shared_ptr<Sprite> create(const std::shared_ptr<Texture2D>& texture);
    explicit Sprite(const Data& data);
    ~Sprite() = default;

    Sprite() = delete;
    Sprite(Sprite&&) = delete;
    Sprite(const Sprite&) = delete;

    [[nodiscard]] const std::shared_ptr<Texture2D>& getTexture() const { return m_texture; }

    [[nodiscard]] const glm::vec3& getColor() const { return m_color; }
    void setColor(const glm::vec3& color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    const std::shared_ptr<Texture2D>& m_texture;

    glm::vec3 m_color;
};
