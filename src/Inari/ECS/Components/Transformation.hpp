#pragma once

#include <glm/vec2.hpp>

namespace inari {
struct Transformation {
    glm::vec2 position;
    float radian;
    glm::vec2 origin;
};
}  // namespace inari