#include "VertexLayout.hpp"

#include "Inari/Assets/Shader.h"

#include "AttributeData.hpp"

using namespace inari;

VertexLayout::VertexLayout(const std::shared_ptr<Shader>& shader, const std::vector<AttributeData>& attributes)
{
    assert(shader != nullptr);
    for (const auto& data : attributes) {
        const int32_t position = shader->getAttributePosition(data.name);
        assert(position != -1);
        this->attributes.emplace(position, data);
    }
}
