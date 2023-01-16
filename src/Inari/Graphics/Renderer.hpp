#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>

namespace inari {
struct VertexLayout;
struct Primitive;

class Renderer {
   public:
    Renderer();
    ~Renderer();

    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void clear(const glm::vec4& color);
    void clear(const glm::vec3& color);

    void setWireframe(bool value);

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
    void enableBlend(BlendParam source, BlendParam destination);

    void activeTexture(uint32_t idx);

    void setVertexLayout(const VertexLayout& layout);

    void drawPrimitive(const Primitive& primitive);

   private:
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;

    std::vector<int32_t> m_attributePositions;
};
}  // namespace inari
