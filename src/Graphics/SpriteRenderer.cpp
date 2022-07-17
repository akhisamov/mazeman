#include "SpriteRenderer.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL.h>

#include <vector>

#include "Resources/Shader.hpp"
#include "Resources/Texture2D.hpp"

#include "Sprite.hpp"

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader)
    : m_quadVAO(0)
    , m_shader(shader)
    , m_view(1.0f)
{
    if (m_shader)
    {
        uint32_t VBO;
        std::vector<float> vertices = { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                                        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };

        glGenVertexArrays(1, &m_quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

        glBindVertexArray(m_quadVAO);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
    }
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &m_quadVAO); }

void SpriteRenderer::begin(const glm::mat4& view)
{
    m_view = view;
    begin();
}

void SpriteRenderer::begin()
{
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f); // TODO move color to argument
    glClear(GL_COLOR_BUFFER_BIT);
}

void SpriteRenderer::draw(const std::shared_ptr<Sprite>& sprite)
{
    if (sprite && m_shader)
    {
        const glm::vec2& origin = sprite->getOrigin();
        const glm::vec2& size = sprite->getTexture()->getSize();

        m_shader->set("model", sprite->getModel());
        m_shader->set("spriteColor", sprite->getColor());
        m_shader->set("view", m_view);

        glActiveTexture(GL_TEXTURE0);
        sprite->getTexture()->bind();

        glBindVertexArray(m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void SpriteRenderer::end() { m_view = glm::mat4(1.0f); }
