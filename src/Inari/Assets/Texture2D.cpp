#include "Texture2D.h"

#include <glad/glad.h>

namespace inari {
    Texture2D::Texture2D(const Data& data)
        : m_id(data.id)
        , m_size(data.size)
    {
    }

    Texture2D::~Texture2D() { glDeleteTextures(1, &m_id); }

    void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }

    uint32_t Texture2D::getId(const std::shared_ptr<Texture2D>& texture)
    {
        if (texture) {
            return texture->getId();
        }
        return 0;
    }
}
