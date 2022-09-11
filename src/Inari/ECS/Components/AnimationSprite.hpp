#pragma once

#include <glm/vec4.hpp>

#include <map>
#include <string>
#include <vector>

#include "Sprite.hpp"

namespace inari
{
    struct AnimationSprite : public Sprite
    {
        using FrameRect = glm::vec4;
        using TrackFrames = std::vector<FrameRect>;
        std::map<std::string, TrackFrames> tracks;
        std::string currentTrack;
        size_t currentFrame = 0;
    };
}