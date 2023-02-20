#pragma once

#include <memory>
#include <string_view>

namespace inari {
    class IAsset;

    class AssetMaker {
    public:
        virtual ~AssetMaker() = default;
        virtual std::shared_ptr<IAsset> createAsset(const std::string_view& data) = 0;
    };
}