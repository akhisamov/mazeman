#pragma once

#include <memory>

class Shader;
class Sprite;

class SpriteRenderer
{
public:
    explicit SpriteRenderer(const std::shared_ptr<Shader>& shader);
    ~SpriteRenderer();

    void draw(const std::shared_ptr<Sprite>& sprite);

private:
    const std::shared_ptr<Shader>& m_shader;
    uint32_t m_quadVAO;
};
