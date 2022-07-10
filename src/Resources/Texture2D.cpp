#include "Texture2D.hpp"

#include <glad/glad.h>

#include <SDL_image.h>

#include <iostream>

#include "StringUtils.hpp"

std::shared_ptr<Texture2D> Texture2D::create(SDL_Surface* surface)
{
    bool success = true;
    uint32_t id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::vec2 size(0.0f);
    if (!surface)
    {
        std::cerr << StringUtils::format("IMG_Load: %s", IMG_GetError()) << std::endl;
        success = false;
    }
    else
    {
        GLenum mode = GL_RGB;
        const uint8_t nOfColors = surface->format->BytesPerPixel;
        switch (nOfColors)
        {
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

        if (success)
        {
            size = glm::vec2(surface->w, surface->h);
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLsizei>(mode), surface->w, surface->h, 0, mode,
                    GL_UNSIGNED_BYTE,
                    surface->pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    glBindBuffer(GL_TEXTURE_2D, 0);
    if (success)
    {
        return std::shared_ptr<Texture2D>(new Texture2D(id, size));
    }
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::createFromData(const std::map<std::string, std::string>& data)
{
    SDL_Surface* surface = nullptr;
    for (const auto& it : data)
    {
        SDL_RWops* rw = SDL_RWFromConstMem(it.second.data(), it.second.size());
        surface = IMG_Load_RW(rw, 1);
        if (surface)
        {
            break;
        }
    }

    if (surface == nullptr)
    {
        const std::string_view message = "Creation Texture2D from data ERROR: %s";
        throw std::runtime_error(StringUtils::format(message, IMG_GetError()));
    }

    return create(surface);
}

std::vector<std::string> Texture2D::getFiles(const std::string_view& name)
{
    const std::vector<std::string_view> exts = { ".jpg" };
    return Resource::getFiles(name, exts);
}

Texture2D::Texture2D(uint32_t id, const glm::vec2& size)
        : m_id(id)
        , m_size(size)
{
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}