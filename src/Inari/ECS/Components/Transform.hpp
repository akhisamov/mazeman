#pragma once

#include <glm/vec2.hpp>

namespace inari {
struct Transform {
    glm::vec2 position;
    float radian = 0.0f;
    glm::vec2 origin;
};
}  // namespace inari