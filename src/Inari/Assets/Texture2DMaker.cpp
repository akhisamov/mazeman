#include "Texture2DMaker.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture2D.h"

namespace inari {
    std::shared_ptr<IAsset> Texture2DMaker::createAsset(const std::string_view& data)
    {
        Texture2D::Data textureData;
        const auto* buffer = reinterpret_cast<const stbi_uc*>(data.data());
        unsigned char* pixels = stbi_load_from_memory(buffer, static_cast<int>(data.size()), &textureData.size.x,
                                                      &textureData.size.y, &textureData.channels, STBI_rgb_alpha);
        if (pixels) {
            glGenTextures(1, &textureData.id);
            glBindTexture(GL_TEXTURE_2D, textureData.id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.size.x, textureData.size.y, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(pixels);
            return std::make_shared<Texture2D>(textureData);
        }

        return nullptr;
    }
}