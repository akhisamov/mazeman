#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace inari {
struct Transform {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 size = glm::vec2(0.0f);
    float radian = 0.0f;
    glm::vec2 origin = glm::vec2(0.0f);
};
}  // namespace inari