#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

namespace inari {
    struct AttributeData;
    class Shader;
    struct VertexLayout {
        std::map<int32_t, AttributeData> attributes;

        explicit VertexLayout(const std::shared_ptr<Shader>& shader, const std::vector<AttributeData>& attributes);

        template <size_t N>
        explicit VertexLayout(const std::shared_ptr<Shader>& shader, const std::array<AttributeData, N>& array)
            : VertexLayout(shader, { array.begin(), array.end() })
        {
        }
    };
} // namespace inari
