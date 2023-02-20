#pragma once

#include "Inari/Assets/AssetMaker.h"

namespace inari {
    class ShaderMaker final : public AssetMaker {
    public:
        std::shared_ptr<IAsset> createAsset(const std::string_view& data) override;
        std::shared_ptr<IAsset> createAsset(const std::string_view& vertexCode, const std::string_view& fragmentCode);
    };
}