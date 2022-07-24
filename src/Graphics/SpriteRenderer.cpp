#include "SpriteRenderer.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Generated/shaders/sprite.hpp"

#include "Resources/Shader.hpp"
#include "Resources/Texture2D.hpp"

#include "Camera2D.hpp"
#include "Sprite.hpp"

SpriteRenderer::SpriteRenderer(const glm::vec2& size)
    : m_shader(Shader::create(shaders::sprite_vert, shaders::sprite_frag))
    , m_view(1.0f)
    , m_vao(0)
    , m_vbo(0)
{
    std::vector<float> vertices = { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                                    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_vao);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    setSize(size);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void SpriteRenderer::setSize(const glm::vec2& size) { m_projection = glm::ortho(0.0f, size.x, size.y, 0.0f); }

void SpriteRenderer::begin(const std::shared_ptr<Camera2D>& camera)
{
    if (camera)
    {
        m_view = camera->getView();
    }

    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SpriteRenderer::draw(const std::shared_ptr<Sprite>& sprite)
{
    if (sprite && m_shader)
    {
        m_shader->use();
        m_shader->set("model", sprite->getModel());
        m_shader->set("view", m_view);
        m_shader->set("projection", m_projection);
        m_shader->set("spriteColor", sprite->getColor());

        glActiveTexture(GL_TEXTURE0);
        sprite->getTexture()->bind();

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void SpriteRenderer::end() { m_view = glm::mat4(1.0f); }
