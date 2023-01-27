#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <map>
#include <memory>
#include <vector>

#include "VertexLayout.hpp"

namespace inari {
    class Texture2D;
    class Shader;
    class Renderer;
    struct SpriteData;

    enum class SpriteSortMode { DEFERRED, IMMEDIATE, TEXTURE };

    class SpriteBatch {
    public:
        explicit SpriteBatch(const std::shared_ptr<Renderer>& renderer, std::shared_ptr<Shader> spriteShader);
        ~SpriteBatch();

        SpriteBatch() = delete;
        SpriteBatch(SpriteBatch&&) = delete;
        SpriteBatch(const SpriteBatch&) = delete;

        void begin(const glm::mat4& transformMatrix = glm::mat4(1.0f),
                   SpriteSortMode sortMode = SpriteSortMode::DEFERRED);

        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect);
        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect,
                  const glm::vec4& sourceRect);
        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect,
                  const glm::vec4& sourceRect, float rotationInRadian, const glm::vec2& origin);

        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position);
        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position,
                  const glm::vec4& sourceRect);
        void draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec2& position,
                  const glm::vec4& sourceRect, float rotationInRadian, const glm::vec2& origin);

        void end();

        void toggleWireframeMode();

    private:
        void flush();
        void flushData(const SpriteData& data);

        std::weak_ptr<Renderer> m_rendererPtr;

        bool m_isBegan;
        bool m_isWireframeMode;

        std::shared_ptr<Shader> m_shader;
        VertexLayout m_layout;

        glm::mat4 m_transformMatrix;
        SpriteSortMode m_sortMode;

        std::vector<SpriteData> m_spriteBuffer;
    };
} // namespace inari
