#pragma once

#include <glm/vec4.hpp>

namespace inari::colors
{
    inline glm::vec4 toGL(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return glm::vec4(r / 255, g / 255, b / 255, a / 255);
    };

    inline glm::vec4 toGL(uint32_t hexValue)
    {
        glm::vec4 result(1.0f);

        for (int i = 0; i < 4; ++i)
        {
            const int shift = 32 - ((i + 1) * 8);
            result[i] = ((hexValue >> shift) & 0xFF) / 255.0f;
        }

        return result;
    };
};
