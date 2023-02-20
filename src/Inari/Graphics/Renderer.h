#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>

#include "Inari/IService.h"

namespace inari {
    struct VertexLayout;
    struct Primitive;
    class Window;

    enum class BlendParam {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE,
        SRC1_COLOR,
        ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA
    };

    class Renderer final : public IService {
        friend class BaseGame;

    public:
        void clear(const glm::vec4& color);
        void clear(const glm::vec3& color);

        void setWireframe(bool value);

        void enableBlend(BlendParam source, BlendParam destination);

        void activeTexture(uint32_t idx);

        void setVertexLayout(const VertexLayout& layout);

        void drawPrimitive(const Primitive& primitive);

    protected:
        struct Token;
        static std::shared_ptr<Renderer> create(const std::shared_ptr<Window>& window);

    public:
        explicit Renderer(const Token& token);
        ~Renderer() override;

        Renderer() = delete;
        Renderer(Renderer&&) = delete;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;

    private:
        void* m_glContext;

        uint32_t m_vao;
        uint32_t m_vbo;
        uint32_t m_ebo;

        std::vector<int32_t> m_attributePositions;
    };
} // namespace inari
