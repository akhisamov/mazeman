#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace inari
{
    class Texture2D;

    struct Sprite
    {
        std::shared_ptr<inari::Texture2D> texture;
        glm::vec2 size;
        glm::vec4 sourceRect;
        glm::vec4 color = glm::vec4(1.0f);
    };
}