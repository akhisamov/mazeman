#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <map>
#include <memory>
#include <vector>

namespace inari {
class Texture2D;
class Shader;
struct SpriteData;

enum class SpriteSortMode { DEFERRED, IMMEDIATE, TEXTURE };

class SpriteBatch {
   public:
    explicit SpriteBatch(std::shared_ptr<Shader> spriteShader);
    ~SpriteBatch();

    SpriteBatch() = delete;
    SpriteBatch(SpriteBatch&&) = delete;
    SpriteBatch(const SpriteBatch&) = delete;

    void begin(const glm::mat4& transformMatrix = glm::mat4(1.0f),
               SpriteSortMode sortMode = SpriteSortMode::DEFERRED);

    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec4& destRect);
    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec4& destRect,
              const glm::vec4& sourceRect);
    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec4& destRect,
              const glm::vec4& sourceRect,
              float rotationInRadian,
              const glm::vec2& origin);

    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec2& position);
    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec2& position,
              const glm::vec4& sourceRect);
    void draw(const std::shared_ptr<Texture2D>& texture,
              const glm::vec4& color,
              const glm::vec2& position,
              const glm::vec4& sourceRect,
              float rotationInRadian,
              const glm::vec2& origin);

    void end();

   private:
    void flush();
    void flushData(const SpriteData& data);

    bool m_isBegan;

    std::shared_ptr<Shader> m_shader;
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;

    glm::mat4 m_transformMatrix;
    SpriteSortMode m_sortMode;

    std::vector<SpriteData> m_spriteBuffer;
};
}  // namespace inari
