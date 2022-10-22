#pragma once

#include "glm/vec2.hpp"

namespace inari {
struct RigidBody {
    glm::vec2 velocity;
    float speed = 0.0f;
};
}  // namespace inari