#pragma once

#include <glm/vec4.hpp>

#include <string>

namespace inari::colors {
inline constexpr glm::vec4 toGL(uint8_t r,
                                uint8_t g,
                                uint8_t b,
                                uint8_t a = 255) {
    return {r / 255, g / 255, b / 255, a / 255};
};

template <int size>
inline constexpr glm::vec<size, float, glm::defaultp> toGL(int hexValue) {
    glm::vec<size, float, glm::defaultp> result(1.0f);

    const int maxSize = size * 8;
    for (int i = 0; i < size; ++i) {
        const int shift = maxSize - ((i + 1) * 8);
        result[i] = ((hexValue >> shift) & 0xFF) / 255.0f;
    }

    return result;
}

template <int size>
inline constexpr glm::vec<size, float, glm::defaultp> toGL(
    const std::string_view& hexValue) {
    assert(!hexValue.empty() && "Color string is empty");
    assert(hexValue.front() == '#' && "Color string is not started with hash");
    assert(hexValue.size() - 1 == size * 2 &&
           "The size of the color string doesn't match the requested size");

    const int num = std::stoi(std::string(hexValue.substr(1)), nullptr, 16);
    return toGL<size>(num);
}

inline constexpr glm::vec4 Black = glm::vec4(0, 0, 0, 1);
inline constexpr glm::vec4 White = glm::vec4(1, 1, 1, 1);
inline constexpr glm::vec4 Red = glm::vec4(1, 0, 0, 1);
inline constexpr glm::vec4 Green = glm::vec4(0, 1, 0, 1);
inline constexpr glm::vec4 Blue = glm::vec4(0, 0, 1, 1);
inline constexpr glm::vec4 Yellow = glm::vec4(1, 1, 0, 1);
inline constexpr glm::vec4 Magenta = glm::vec4(1, 0, 1, 1);
inline constexpr glm::vec4 Cyan = glm::vec4(0, 1, 1, 1);
};  // namespace inari::colors
