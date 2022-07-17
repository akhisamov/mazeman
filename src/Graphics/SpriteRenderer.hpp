#pragma once

#include <glm/mat4x4.hpp>

#include <memory>

class Shader;
class Sprite;

class SpriteRenderer
{
public:
    explicit SpriteRenderer(const std::shared_ptr<Shader>& shader);
    ~SpriteRenderer();

    void begin(const glm::mat4& view);
    void begin();
    void draw(const std::shared_ptr<Sprite>& sprite);
    void end();

private:
    const std::shared_ptr<Shader>& m_shader;
    uint32_t m_quadVAO;
    glm::mat4 m_view;
};
