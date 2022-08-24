#include "SpriteBatch.hpp"

#include <glad/glad.h>

#include "Resources/Texture2D.hpp"
#include "Resources/Shader.hpp"

/* TODO
 * Enum SpriteSortMode
 * {
 *      Deferred,   // All sprites are drawing when End() invokes,
 *                  // in order of draw call sequence.
 *      Immediate,  // Each sprite is drawing at individual draw call,
 *                  // instead of End().
 *      Texture,    // Same as Deferred, except sprites are sorted by
 *                  // texture prior to drawing.
 * }
 */

struct SpriteData
{
    const std::shared_ptr<Texture2D>& texture;

    struct VertexData
    {
        glm::vec2 position;
        glm::vec2 uv;
        glm::vec4 color;

        VertexData(const glm::vec2& position, const glm::vec2& uv, const glm::vec4& color)
            : position(position)
            , uv(uv)
            , color(color)
        {
        }
    };
    std::vector<VertexData> vertices;

    SpriteData(const std::shared_ptr<Texture2D>& texture)
        : texture(texture)
    {
    }
};

constexpr std::size_t sizeOfVertexData = sizeof(SpriteData::VertexData);

SpriteBatch::SpriteBatch(const std::shared_ptr<Shader>& spriteShader)
    : m_isBegan(false)
    , m_shader(spriteShader)
    , m_vao(0)
    , m_vbo(0)
    , m_transformMatrix(1.0f)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeOfVertexData, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeOfVertexData, (void*)sizeof(glm::vec2));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeOfVertexData, (void*)sizeof(glm::vec4));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteBatch::~SpriteBatch()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void SpriteBatch::begin(const glm::mat4& transformMatrix)
{
    m_transformMatrix = transformMatrix;

    // TODO
    // glBlend

    m_isBegan = true;
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect)
{
    if (texture != nullptr)
    {
        const glm::vec4 sourceRect(0.0f, 0.0f, texture->getSize());
        draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
    }
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect, const glm::vec4& sourceRect)
{
    draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect,
              const glm::vec4& sourceRect, float rotationInRadian, const glm::vec2& origin)
{
    // TODO
    // how to rotate vertices?
    if (!m_isBegan || m_shader == nullptr || m_vbo == 0 || texture == nullptr)
    {
        return;
    }

    SpriteData data(texture);
    data.vertices.emplace_back(glm::vec2(destRect.x, destRect.y), glm::vec2(sourceRect.x, sourceRect.y), color);
    data.vertices.emplace_back(glm::vec2(destRect.x + destRect.z, destRect.y), glm::vec2(sourceRect.z, sourceRect.y), color);
    data.vertices.emplace_back(glm::vec2(destRect.x, destRect.y + destRect.w), glm::vec2(sourceRect.x, sourceRect.w), color);
    data.vertices.emplace_back(glm::vec2(destRect.x + destRect.z, destRect.y), glm::vec2(sourceRect.z, sourceRect.y), color);
    data.vertices.emplace_back(glm::vec2(destRect.x, destRect.y + destRect.w), glm::vec2(sourceRect.x, sourceRect.w), color);
    data.vertices.emplace_back(glm::vec2(destRect.x + destRect.z, destRect.y + destRect.w), glm::vec2(sourceRect.z, sourceRect.w), color);

    m_spriteBuffer.push_back(data);
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position)
{
    draw(texture, color, position, glm::vec4(0.0f), 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position,
              const glm::vec4& sourceRect)
{
    draw(texture, color, position, sourceRect, 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position,
              const glm::vec4& sourceRect, float rotationInRadian, const glm::vec2& origin)
{
    if (texture != nullptr)
    {
        if (sourceRect == glm::vec4(0.0f))
        {
            const glm::vec4 defaultSourceRect(0.0f, 0.0f, texture->getSize());
            const glm::vec4 destRect(position, texture->getSize());
            draw(texture, color, destRect, defaultSourceRect, rotationInRadian, origin);
        }
        else
        {
            const glm::vec4 destRect(position, sourceRect.z, sourceRect.w);
            draw(texture, color, destRect, sourceRect, rotationInRadian, origin);
        }
    }
}

void SpriteBatch::end()
{
    m_isBegan = false;

    flush();
}

void SpriteBatch::flush()
{
    if (m_shader == nullptr || m_spriteBuffer.empty() || m_vbo == 0)
    {
        return;
    }

    m_shader->use();
    m_shader->set("transform", m_transformMatrix);

    glBindVertexArray(m_vao);
    for (const auto& it : m_spriteBuffer)
    {
        if (it.texture != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            it.texture->bind();
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeOfVertexData * it.vertices.size(), it.vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(it.vertices.size()));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
    glBindVertexArray(0);

    m_spriteBuffer.clear();
}
