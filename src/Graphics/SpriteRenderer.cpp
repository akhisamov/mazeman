#include "SpriteRenderer.hpp"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Generated/shaders/sprite.hpp"

#include "Resources/Shader.hpp"
#include "Resources/Texture2D.hpp"

#include "Camera2D.hpp"
#include "Sprite.hpp"

SpriteRenderer::SpriteRenderer()
    : m_shader(Shader::create(shaders::sprite_vert, shaders::sprite_frag))
    , m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_cameraMatrix(1.0f)
{
    std::vector<glm::vec2> vertices = { { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } };
    uint32_t indices[] = { 0, 2, 3, 0, 1, 2 };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void SpriteRenderer::begin(const glm::vec4 clearColor, const std::shared_ptr<Camera2D>& camera)
{
    if (camera)
    {
        m_cameraMatrix = camera->getMatrix();
    }

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SpriteRenderer::draw(const std::shared_ptr<Sprite>& sprite)
{
    if (sprite && m_shader)
    {
        m_shader->use();
        m_shader->set("MVP", m_cameraMatrix * sprite->getModel());
        m_shader->set("spriteColor", sprite->getColor());

        glActiveTexture(GL_TEXTURE0);
        sprite->getTexture()->bind();

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void SpriteRenderer::end() { m_cameraMatrix = glm::mat4(1.0f); }
