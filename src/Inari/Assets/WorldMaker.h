#pragma once

#include "Inari/Assets/AssetMaker.h"

namespace inari {
    class WorldMaker final : public AssetMaker {
    public:
        std::shared_ptr<IAsset> createAsset(const std::string_view& data) override;
    };
}