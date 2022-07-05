#include "SpriteRenderer.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>

#include <vector>

#include "Resources/Texture2D.hpp"
#include "Resources/Shader.hpp"

#include "Sprite.hpp"

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader)
        :
        m_quadVAO(0),
        m_shader(shader)
{
    if (m_shader)
    {
        uint32_t VBO;
        std::vector<float> vertices = {
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
        };

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

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::draw(const std::shared_ptr<Sprite>& sprite)
{
    if (sprite && m_shader)
    {
        const glm::vec2& origin = sprite->getOrigin();
        const glm::vec2& size = sprite->getTexture()->getSize();

        m_shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(sprite->getPosition(), 0.0f));

        model = glm::rotate(model, glm::radians(sprite->getDegrees()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.0f));

        model = glm::scale(model, glm::vec3(size, 1.0f));

        m_shader->set("model", model);
        m_shader->set("spriteColor", sprite->getColor());

        glActiveTexture(GL_TEXTURE0);
        sprite->getTexture()->bind();

        glBindVertexArray(m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}
