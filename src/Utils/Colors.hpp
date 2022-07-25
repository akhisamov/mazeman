#pragma once

#include <glm/vec4.hpp>

class Colors
{
public:
    static glm::vec4 toGL(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return glm::vec4(r / 255, g / 255, b / 255, a / 255);
    };

    static glm::vec4 toGL(int hexValue)
    {
        glm::vec4 result(1.0f);
        result.r = ((hexValue >> 16) & 0xFF) / 255.0f;
        result.g = ((hexValue >> 8) & 0xFF) / 255.0f;
        result.b = (hexValue & 0xFF) / 255.0f;
        return result;
    };
};
