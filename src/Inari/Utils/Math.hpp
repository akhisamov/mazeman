#pragma once

namespace inari::math
{
    inline constexpr float pi = 3.14159274101257324219;

    inline float degreesToRadians(float degrees) { return degrees * (pi / 180); }
    inline float radiansToDegrees(float radians) { return radians * (180 / pi); }
}