#include "Texture2D.hpp"

#include <glad/glad.h>

#include <SDL_image.h>

#include <iostream>

#include "Inari/Utils/Strings.hpp"

namespace inari {
std::shared_ptr<Texture2D> Texture2D::create(SDL_Surface* surface) {
    Texture2D::Data data;
    bool success = true;
    glGenTextures(1, &data.id);

    glBindTexture(GL_TEXTURE_2D, data.id);

    if (!surface) {
        std::cerr << strings::format("IMG_Load: %s", IMG_GetError())
                  << std::endl;
        success = false;
    } else {
        GLenum mode = GL_RGB;
        const uint8_t nOfColors = surface->format->BytesPerPixel;
        switch (nOfColors) {
            case 4:
                if (surface->format->Rmask == 0x000000ff)
                    mode = GL_RGBA;
                else
                    mode = GL_BGRA;
                break;
            case 3:
                if (surface->format->Rmask == 0x000000ff)
                    mode = GL_RGB;
                else
                    mode = GL_BGR;
                break;
            default:
                std::cerr << "Error, image is not truecolor." << std::endl;
                success = false;
        }

        if (success) {
            data.size = glm::vec2(surface->w, surface->h);
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLsizei>(mode),
                         surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE,
                         surface->pixels);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    if (success) {
        return std::make_shared<Texture2D>(data);
    }
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::createFromData(
    const std::map<std::string, std::string>& data) {
    SDL_Surface* surface = nullptr;
    for (const auto& it : data) {
        SDL_RWops* rw = SDL_RWFromConstMem(it.second.data(),
                                           static_cast<int>(it.second.size()));
        surface = IMG_Load_RW(rw, 1);
        if (surface) {
            break;
        }
    }

    if (surface == nullptr) {
        return nullptr;
    }

    return create(surface);
}

Texture2D::Texture2D(const Data& data) : m_id(data.id), m_size(data.size) {}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_id);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_id);
}

uint32_t Texture2D::getId(const std::shared_ptr<Texture2D>& texture) {
    if (texture) {
        return texture->getId();
    }
    return 0;
}
}  // namespace inari
