#include "SpriteBatch.hpp"

#include <glad/glad.h>

#include <glm/ext/matrix_transform.hpp>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <set>
#include <utility>

#include "Inari/Resources/Shader.hpp"
#include "Inari/Resources/Texture2D.hpp"

namespace {
glm::mat2 getAngleMatrix(float angle) {
    const float s = std::sin(angle);
    const float c = std::cos(angle);
    return {c, s, -s, c};
}
}  // namespace

namespace inari {
struct SpriteData {
    std::shared_ptr<Texture2D> texture;

    struct VertexData {
        struct PositionGenerator {
            glm::vec2 origin;
            glm::vec2 rectSize;
            glm::vec2 rectPosition;
            glm::mat2 angleMatrix;

            PositionGenerator(const glm::vec2& origin,
                              const glm::vec4& rect,
                              float angle)
                : origin(origin),
                  rectSize(rect.w, rect.z),
                  rectPosition(rect.x, rect.y),
                  angleMatrix(getAngleMatrix(angle)) {}

            glm::vec2 make(const glm::vec2& position) const {
                glm::vec2 result = position;
                result = result - (origin * rectSize);
                result = result - rectPosition;
                return (angleMatrix * result) + rectPosition;
            }
        };

        glm::vec2 position;
        glm::vec2 uv;

        VertexData(const glm::vec2& position,
                   const glm::vec2& uv,
                   const PositionGenerator& generator)
            : position(generator.make(position)), uv(uv) {}
    };
    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;

    glm::vec4 color = glm::vec4(1.0f);

    SpriteData() : texture(nullptr) {}

    explicit SpriteData(std::shared_ptr<Texture2D> texture)
        : texture(std::move(texture)) {}
};

SpriteBatch::SpriteBatch(std::shared_ptr<Shader> spriteShader)
    : m_isBegan(false),
      m_isWireframeMode(false),
      m_shader(std::move(spriteShader)),
      m_vao(0),
      m_vbo(0),
      m_ebo(0),
      m_transformMatrix(1.0f),
      m_sortMode(SpriteSortMode::DEFERRED) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(SpriteData::VertexData), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(SpriteData::VertexData),
                          (void*)sizeof(glm::vec2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteBatch::~SpriteBatch() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void SpriteBatch::begin(const glm::mat4& transformMatrix,
                        SpriteSortMode sortMode) {
    m_transformMatrix = transformMatrix;
    m_sortMode = sortMode;
    m_isBegan = true;
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec4& destRect) {
    if (texture != nullptr) {
        const glm::vec4 sourceRect(0.0f, 0.0f, texture->getSize());
        draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
    }
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec4& destRect,
                       const glm::vec4& sourceRect) {
    draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec4& destRect,
                       const glm::vec4& sourceRect,
                       float rotationInRadian,
                       const glm::vec2& origin) {
    if (!m_isBegan || m_shader == nullptr || m_vbo == 0 || m_vao == 0 ||
        texture == nullptr) {
        return;
    }

    const SpriteData::VertexData::PositionGenerator generator(origin, destRect,
                                                              rotationInRadian);

    SpriteData data(texture);
    data.vertices.emplace_back(glm::vec2(destRect.x, destRect.y),
                               glm::vec2(sourceRect.x, sourceRect.y),
                               generator);  // 0
    data.vertices.emplace_back(
        glm::vec2(destRect.x + destRect.z, destRect.y),
        glm::vec2(sourceRect.x + sourceRect.z, sourceRect.y), generator);  // 1
    data.vertices.emplace_back(
        glm::vec2(destRect.x, destRect.y + destRect.w),
        glm::vec2(sourceRect.x, sourceRect.y + sourceRect.w), generator);  // 2
    data.vertices.emplace_back(
        glm::vec2(destRect.x + destRect.z, destRect.y + destRect.w),
        glm::vec2(sourceRect.x + sourceRect.z, sourceRect.y + sourceRect.w),
        generator);  // 3
    data.indices = {0, 1, 2, 1, 2, 3};

    data.color = color;

    m_spriteBuffer.push_back(data);

    if (m_sortMode == SpriteSortMode::IMMEDIATE) {
        flush();
    }
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec2& position) {
    draw(texture, color, position, glm::vec4(0.0f), 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec2& position,
                       const glm::vec4& sourceRect) {
    draw(texture, color, position, sourceRect, 0.0f, glm::vec2(0.0f));
}

void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture,
                       const glm::vec4& color,
                       const glm::vec2& position,
                       const glm::vec4& sourceRect,
                       float rotationInRadian,
                       const glm::vec2& origin) {
    if (texture != nullptr) {
        const glm::vec4 sRect = sourceRect != glm::vec4(0.0f)
                                    ? sourceRect
                                    : glm::vec4(0.0f, 0.0f, texture->getSize());
        const glm::vec4 destRect(position, texture->getSize());
        draw(texture, color, destRect, sRect, rotationInRadian, origin);
    }
}

void SpriteBatch::end() {
    m_isBegan = false;

    flush();
}

void SpriteBatch::toggleWireframeMode() {
    m_isWireframeMode = !m_isWireframeMode;
    glPolygonMode(GL_FRONT_AND_BACK, m_isWireframeMode ? GL_LINE : GL_FILL);
}

void SpriteBatch::flush() {
    if (m_shader == nullptr || m_spriteBuffer.empty() || m_vbo == 0 ||
        m_vao == 0) {
        return;
    }

    std::vector<size_t> texturesOrder;
    if (m_sortMode == SpriteSortMode::TEXTURE) {
        std::set<uint32_t> textureIds;
        for (size_t i = 0; i < m_spriteBuffer.size(); ++i) {
            const uint32_t masterTextureId =
                Texture2D::getId(m_spriteBuffer[i].texture);
            if (masterTextureId != 0 &&
                textureIds.find(masterTextureId) == textureIds.end()) {
                for (size_t j = 0; j < m_spriteBuffer.size(); ++j) {
                    const uint32_t textureId =
                        Texture2D::getId(m_spriteBuffer[j].texture);
                    if (textureId == masterTextureId) {
                        texturesOrder.push_back(j);
                    }
                }
                textureIds.insert(masterTextureId);
            }
        }
    }

    // AlphaBlend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader->use();
    m_shader->set("transform", m_transformMatrix);

    glBindVertexArray(m_vao);
    if (m_sortMode == SpriteSortMode::TEXTURE && !texturesOrder.empty()) {
        for (const size_t index : texturesOrder) {
            if (m_spriteBuffer.size() > index) {
                const SpriteData& data = m_spriteBuffer[index];
                flushData(data);
            }
        }
    } else {
        for (const SpriteData& data : m_spriteBuffer) {
            flushData(data);
        }
    }
    glBindVertexArray(0);

    m_spriteBuffer.clear();
}

void SpriteBatch::flushData(const SpriteData& data) {
    if (data.texture != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        data.texture->bind();
        m_shader->set("image", 0);
    }

    m_shader->set("color", data.color);

    const auto verticesSize = static_cast<GLsizeiptr>(
        sizeof(SpriteData::VertexData) * data.vertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, data.vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    const auto indicesSize =
        static_cast<GLsizeiptr>(sizeof(uint32_t) * data.indices.size());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, data.indices.data(),
                 GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
}  // namespace inari
