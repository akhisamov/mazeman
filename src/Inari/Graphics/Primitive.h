#pragma once

#include <vector>

namespace inari {
    enum class PrimitiveType { NONE = 0, LINES, LINE_STRIP, POINTS, TRIANGLE_FAN, TRIANGLES, TRIANGLE_STRIP };

    struct Primitive {
        using VerticesType = std::vector<float>;
        using IndicesType = std::vector<uint32_t>;

        PrimitiveType type;
        VerticesType vertices;
        IndicesType indices;

        Primitive(PrimitiveType type, VerticesType vertices, IndicesType indices)
            : type(type)
            , vertices(std::move(vertices))
            , indices(std::move(indices))
        {
        }
    };
} // namespace inari
