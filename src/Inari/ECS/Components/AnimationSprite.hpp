#pragma once

#include <glm/vec4.hpp>

#include <map>
#include <string>
#include <vector>

namespace inari {
struct AnimationSprite {
    std::string currentTrack;
    size_t currentFrame = 0;

    bool isFramesLimited = false;
    float framesLimit = 0.0f;

    using FrameRect = glm::vec4;
    using TrackFrames = std::vector<FrameRect>;
    std::map<std::string, TrackFrames> tracks;

    float lastUpdate = 0.0f;
};
}  // namespace inari