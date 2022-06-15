#include "SpriteRenderer.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL.h>

#include <vector>

#include "Texture2D.hpp"
#include "Shader.hpp"

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader) :
    m_quadVAO(0),
    m_shader(shader)
{
    if (m_shader)
    {
        m_shader->use();
        m_shader->set("projection", glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f));

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

void SpriteRenderer::draw(const std::shared_ptr<Texture2D>& texture)
{
    if (texture && m_shader)
    {
        m_shader->use();
        glm::vec2 position(0.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
//        model = glm::translate(model, glm::vec3(0.5f * 200, 0.5f * 200, 0.0f));  // move origin of rotation to center of quad
//        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
//        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
        model = glm::scale(model, glm::vec3(texture->getSize(), 1.0f)); // last scale
        m_shader->set("model", model);

        m_shader->set("spriteColor", glm::vec3(1.0f));

        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        glBindVertexArray(m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}
