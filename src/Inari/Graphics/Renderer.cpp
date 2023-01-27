#include "Renderer.hpp"

#include "AttributeData.hpp"
#include "Primitive.hpp"
#include "VertexLayout.hpp"

#include <glad/glad.h>

using namespace inari;

namespace {
    constexpr GLenum cast(PrimitiveType type)
    {
        switch (type) {
        case PrimitiveType::LINES:
            return GL_LINES;
        case PrimitiveType::LINE_STRIP:
            return GL_LINE_STRIP;
        case PrimitiveType::POINTS:
            return GL_POINTS;
        case PrimitiveType::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case PrimitiveType::TRIANGLES:
            return GL_TRIANGLES;
        case PrimitiveType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::NONE:
        default:
            return GL_NONE;
        }
    }
    constexpr GLenum cast(BlendParam value)
    {
        using B = BlendParam;
        switch (value) {
        case B::ZERO:
            return GL_ZERO;
        case B::ONE:
            return GL_ONE;
        case B::SRC_COLOR:
            return GL_SRC_COLOR;
        case B::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case B::DST_COLOR:
            return GL_DST_COLOR;
        case B::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case B::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case B::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case B::DST_ALPHA:
            return GL_DST_ALPHA;
        case B::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case B::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case B::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case B::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case B::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case B::SRC_ALPHA_SATURATE:
            return GL_SRC_ALPHA_SATURATE;
        case B::SRC1_COLOR:
            return GL_SRC1_COLOR;
        case B::ONE_MINUS_SRC1_COLOR:
            return GL_ONE_MINUS_SRC1_COLOR;
        case B::SRC1_ALPHA:
            return GL_SRC1_ALPHA;
        case B::ONE_MINUS_SRC1_ALPHA:
            return GL_ONE_MINUS_SRC1_ALPHA;
        }
    }
} // namespace

Renderer::Renderer()
    : m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
}
Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Renderer::clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear(const glm::vec3& color) { clear({ color, 1.0f }); }

void Renderer::setWireframe(bool value) { glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL); }

void Renderer::enableBlend(BlendParam source, BlendParam destination)
{
    glEnable(GL_BLEND);
    glBlendFunc(cast(source), cast(destination));
}

void Renderer::activeTexture(uint32_t idx)
{
    assert(idx < 32);
    glActiveTexture(GL_TEXTURE0 + idx);
}

void Renderer::setVertexLayout(const VertexLayout& layout)
{
    m_attributePositions.clear();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    for (auto [position, data] : layout.attributes) {
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, data.size, GL_FLOAT, data.normalized, data.stride,
                              reinterpret_cast<void*>(data.offset));
        glDisableVertexAttribArray(position);
        m_attributePositions.push_back(position);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::drawPrimitive(const Primitive& primitive)
{
    glBindVertexArray(m_vao);
    const auto verticesSize = static_cast<GLsizeiptr>(sizeof(float) * primitive.vertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, primitive.vertices.data(), GL_STATIC_DRAW);

    std::for_each(m_attributePositions.begin(), m_attributePositions.end(), glEnableVertexAttribArray);

    const auto indicesSize
        = static_cast<GLsizeiptr>(sizeof(decltype(primitive.indices)::value_type) * primitive.indices.size());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, primitive.indices.data(), GL_STATIC_DRAW);
    glDrawElements(cast(primitive.type), 6, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::for_each(m_attributePositions.rbegin(), m_attributePositions.rend(), glDisableVertexAttribArray);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
