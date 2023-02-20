#include "Renderer.h"

#include <glad/glad.h>

#include <SDL.h>

#include "AttributeData.h"
#include "Primitive.h"
#include "VertexLayout.h"
#include "Window.h"

namespace {
    constexpr GLenum cast(inari::PrimitiveType type)
    {
        using T = inari::PrimitiveType;
        switch (type) {
        case T::LINES:
            return GL_LINES;
        case T::LINE_STRIP:
            return GL_LINE_STRIP;
        case T::POINTS:
            return GL_POINTS;
        case T::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case T::TRIANGLES:
            return GL_TRIANGLES;
        case T::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case T::NONE:
        default:
            return GL_NONE;
        }
    }
    constexpr GLenum cast(inari::BlendParam value)
    {
        using B = inari::BlendParam;
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

namespace inari {
    struct Renderer::Token {
        SDL_GLContext glContext = nullptr;
    };

    std::shared_ptr<Renderer> Renderer::create(const std::shared_ptr<Window>& window)
    {
        if (window == nullptr) {
            return nullptr;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        auto* sdlWindow = reinterpret_cast<SDL_Window*>(window->getWindow());
        if (sdlWindow == nullptr) {
            return nullptr;
        }

        Token token;
        token.glContext = SDL_GL_CreateContext(sdlWindow);
        if (token.glContext == nullptr) {
            return nullptr;
        }

        if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
            SDL_GL_DeleteContext(token.glContext);
            return nullptr;
        }
        return std::make_shared<Renderer>(token);
    }

    Renderer::Renderer(const Token& token)
        : m_glContext(token.glContext)
        , m_vao(0)
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
        SDL_GL_DeleteContext(reinterpret_cast<SDL_GLContext>(m_glContext));
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
}