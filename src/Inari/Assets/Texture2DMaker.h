#pragma once

#include "AssetMaker.h"

namespace inari {
    class Texture2DMaker final : public AssetMaker {
    public:
        std::shared_ptr<IAsset> createAsset(const std::string_view& data) override;
    };
} // inari