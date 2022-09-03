#pragma once

namespace math
{
    constexpr float pi = 3.14159274101257324219;

    float degreesToRadians(float degrees) { return degrees * (pi / 180); }
    float radiansToDegrees(float radians) { return radians * (180 / pi); }
}