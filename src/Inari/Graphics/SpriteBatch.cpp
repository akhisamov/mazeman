#include "SpriteBatch.h"

#include <glm/ext/matrix_transform.hpp>

#include <set>

#include "AttributeData.h"
#include "Primitive.h"
#include "Renderer.h"
#include "VertexLayout.h"

#include "Inari/Assets/Shader.h"
#include "Inari/Assets/ShaderMaker.h"
#include "Inari/Assets/Shaders/SpriteShader.h"
#include "Inari/Assets/Texture2D.h"

namespace {
    constexpr std::string_view IN_POSITION_STR = "in_position";
    constexpr std::string_view IN_UV_STR = "in_uv";

    constexpr inari::AttributeData makeAttributeData(const std::string_view& name)
    {
        assert(name == IN_POSITION_STR || name == IN_UV_STR);
        constexpr auto stride = sizeof(glm::vec2) * 2;
        return { name, 2, stride, name == IN_UV_STR ? sizeof(glm::vec2) : 0 };
    }

    constexpr auto ATTRIBUTES = inari::makeAttributes<2, makeAttributeData>({ IN_POSITION_STR, IN_UV_STR });
} // namespace

namespace inari {
    struct SpriteData {
        static constexpr std::initializer_list<float> VERTICES {
            // pos      uv
            0.0f, 0.0f, 0.0f, 0.0f, // 0
            1.0f, 0.0f, 1.0f, 0.0f, // 1
            0.0f, 1.0f, 0.0f, 1.0f, // 2
            1.0f, 1.0f, 1.0f, 1.0f // 3
        };
        static constexpr std::initializer_list<uint32_t> INDICES { 0, 1, 2, 1, 2, 3 };

        std::shared_ptr<Texture2D> texture;
        glm::vec4 color = glm::vec4(1.0f);
        glm::mat4 model;
        glm::mat4 uvModel;
        Primitive primitive;

        explicit SpriteData(std::shared_ptr<Texture2D> texture, const glm::mat4& model, const glm::mat4 uvModel,
                            const glm::vec4& color)
            : texture(std::move(texture))
            , model(model)
            , uvModel(uvModel)
            , color(color)
            , primitive(PrimitiveType::TRIANGLES, VERTICES, INDICES)
        {
        }
    };

    std::shared_ptr<SpriteBatch> SpriteBatch::create(const std::shared_ptr<Renderer>& renderer)
    {
        return std::make_shared<SpriteBatch>(Token { renderer });
    }

    SpriteBatch::SpriteBatch(const Token& token)
        : m_rendererPtr(token.weakRenderer)
        , m_isBegan(false)
        , m_isWireframeMode(false)
        , m_shader(std::dynamic_pointer_cast<Shader>(ShaderMaker {}.createAsset(shaders::sprite)))
        , m_layout(std::make_unique<VertexLayout>(m_shader, ATTRIBUTES))
        , m_transformMatrix(1.0f)
        , m_sortMode(SpriteSortMode::DEFERRED)
    {
    }

    SpriteBatch::~SpriteBatch() = default;

    void SpriteBatch::begin(const glm::mat4& transformMatrix, SpriteSortMode sortMode)
    {
        const auto& renderer = m_rendererPtr.lock();
        if (renderer == nullptr || m_layout == nullptr) {
            return;
        }

        m_transformMatrix = transformMatrix;
        m_sortMode = sortMode;
        m_isBegan = true;

        renderer->setVertexLayout(*m_layout);
    }

    void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect)
    {
        if (texture != nullptr) {
            const glm::vec4 sourceRect(0.0f, 0.0f, texture->getSize());
            draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
        }
    }

    void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect,
                           const glm::vec4& sourceRect)
    {
        draw(texture, color, destRect, sourceRect, 0.0f, glm::vec2(0.0f));
    }

    void SpriteBatch::draw(const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const glm::vec4& destRect,
                           const glm::vec4& sourceRect, float rotationInRadian, const glm::vec2& origin)
    {
        if (!m_isBegan || m_shader == nullptr || texture == nullptr) {
            return;
        }

        const glm::vec2 position(destRect.x, destRect.y);
        const glm::vec2 size(destRect.z, destRect.w);

        // T
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

        // R
        model = glm::rotate(model, rotationInRadian, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.0f));

        // S
        model = glm::scale(model, glm::vec3(size, 1.0f));

        auto uvModel = glm::translate(glm::mat4(1.0f), glm::vec3(sourceRect.x, sourceRect.y, 0.0f));
        uvModel = glm::scale(uvModel, glm::vec3(sourceRect.z, sourceRect.w, 1.0f));

        m_spriteBuffer.emplace_back(texture, model, uvModel, color);

        if (m_sortMode == SpriteSortMode::IMMEDIATE) {
            flush();
        }
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
        if (texture != nullptr) {
            const glm::vec4 sRect
                = sourceRect != glm::vec4(0.0f) ? sourceRect : glm::vec4(0.0f, 0.0f, texture->getSize());
            const glm::vec4 destRect(position, texture->getSize());
            draw(texture, color, destRect, sRect, rotationInRadian, origin);
        }
    }

    void SpriteBatch::end()
    {
        m_isBegan = false;

        flush();
    }

    void SpriteBatch::toggleWireframeMode()
    {
        m_isWireframeMode = !m_isWireframeMode;

        auto renderer = m_rendererPtr.lock();
        renderer->setWireframe(m_isWireframeMode);
    }

    void SpriteBatch::flush()
    {
        assert(m_shader);

        std::vector<size_t> texturesOrder;
        if (m_sortMode == SpriteSortMode::TEXTURE) {
            std::set<uint32_t> textureIds;
            for (size_t i = 0; i < m_spriteBuffer.size(); ++i) {
                const uint32_t masterTextureId = Texture2D::getId(m_spriteBuffer[i].texture);
                if (masterTextureId != 0 && textureIds.find(masterTextureId) == textureIds.end()) {
                    for (size_t j = 0; j < m_spriteBuffer.size(); ++j) {
                        const uint32_t textureId = Texture2D::getId(m_spriteBuffer[j].texture);
                        if (textureId == masterTextureId) {
                            texturesOrder.push_back(j);
                        }
                    }
                    textureIds.insert(masterTextureId);
                }
            }
        }

        auto renderer = m_rendererPtr.lock();
        // AlphaBlend
        renderer->enableBlend(BlendParam::SRC_ALPHA, BlendParam::ONE_MINUS_SRC_ALPHA);

        m_shader->use();
        m_shader->set("transform", m_transformMatrix);

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

        m_spriteBuffer.clear();
    }

    void SpriteBatch::flushData(const SpriteData& data)
    {
        auto renderer = m_rendererPtr.lock();

        if (data.texture != nullptr) {
            renderer->activeTexture(0);
            data.texture->bind();
            m_shader->set("image", 0);
        }

        m_shader->set("color", data.color);
        m_shader->set("model", data.model);
        m_shader->set("uv_model", data.uvModel);

        renderer->drawPrimitive(data.primitive);
    }
} // namespace inari
