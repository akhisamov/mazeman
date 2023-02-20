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
        friend class BaseGame;

        using AssetID = std::size_t;

    public:
        void addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint = "");
        void addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths);

        template <class T>
        std::shared_ptr<T> load(const std::string_view& name);
        bool unload(const std::string_view& name);
        bool has(const std::string_view& name);

        template <class T>
        void registerMaker(std::unique_ptr<AssetMaker>&& maker);
        template <class T>
        const std::unique_ptr<AssetMaker>& getMaker() const;

    protected:
        struct Token { };
        static std::shared_ptr<AssetsManager> create();

    private:
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
        std::map<std::size_t, std::unique_ptr<AssetMaker>> m_makers;
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

        const std::unique_ptr<AssetMaker>& maker = getMaker<T>();
        if (maker) {
            const auto& it = m_assets.emplace(id, maker->createAsset(readFileData(name)));
            if (it.second) {
                return std::dynamic_pointer_cast<T>(it.first->second);
            }
        }
        return nullptr;
    }

    template <class T>
    void AssetsManager::registerMaker(std::unique_ptr<AssetMaker>&& maker)
    {
        static_assert(std::is_base_of_v<IAsset, T>);
        const size_t hash = typeid(T).hash_code();
        const auto& it = m_makers.find(hash);
        if (it == m_makers.end()) {
            m_makers.emplace(hash, std::move(maker));
        }
    }

    template <class T>
    const std::unique_ptr<AssetMaker>& AssetsManager::getMaker() const
    {
        static_assert(std::is_base_of_v<IAsset, T>);
        const size_t hash = typeid(T).hash_code();
        const auto& it = m_makers.find(hash);
        assert(it != m_makers.end());
        return it->second;
    }
}