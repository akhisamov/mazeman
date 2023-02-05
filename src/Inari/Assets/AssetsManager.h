#pragma once

#include <map>
#include <memory>
#include <string>

#include "Inari/Assets/AssetMaker.h"
#include "Inari/IService.h"

namespace inari {
    class IAsset;
    class AssetMaker;

    class AssetsManager final : public IService {
        friend class IGame;

        using AssetID = std::size_t;

    public:
        void addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint = "");
        void addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths);

        template <class T>
        std::shared_ptr<T> load(const std::string_view& name);
        bool unload(const std::string_view& name);
        bool has(const std::string_view& name);

    protected:
        struct Token { };
        static std::shared_ptr<AssetsManager> create();

    private:
        template <class T>
        std::shared_ptr<AssetMaker> getMaker();

        std::string readFileData(const std::string_view& filename);

    public:
        explicit AssetsManager(Token);
        ~AssetsManager() override;

        AssetsManager() = delete;
        AssetsManager(const AssetsManager&) = delete;
        AssetsManager(AssetsManager&&) = delete;
        AssetsManager& operator=(const AssetsManager&) = delete;
        AssetsManager& operator=(AssetsManager&&) = delete;

    private:
        std::map<std::string, std::string> m_filesDataByName;
        std::map<AssetID, std::shared_ptr<IAsset>> m_assets;
    };

    template <class T>
    std::shared_ptr<T> AssetsManager::load(const std::string_view& name)
    {
        static_assert(std::is_base_of_v<IAsset, T>);

        const AssetID id = std::hash<std::string_view> {}(name);
        {
            const auto& it = m_assets.find(id);
            if (it != m_assets.end()) {
                return std::dynamic_pointer_cast<T>(it->second);
            }
        }

        std::shared_ptr<AssetMaker> maker = getMaker<T>();
        if (maker) {
            const auto& it = m_assets.emplace(id, maker->createAsset(readFileData(name)));
            if (it.second) {
                return std::dynamic_pointer_cast<T>(it.first->second);
            }
        }
        return nullptr;
    }

    template <class T>
    std::shared_ptr<AssetMaker> AssetsManager::getMaker()
    {
        return nullptr;
    }
}