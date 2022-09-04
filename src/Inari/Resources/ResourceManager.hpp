#pragma once

#include <vector>

#include "Inari/Resources/ResourceStorage.hpp"
#include "Inari/Resources/Shader.hpp"
#include "Inari/Resources/Texture2D.hpp"
#include "Inari/Utils/Strings.hpp"

namespace inari
{
    class ResourceManager final
        : public ResourceStorage<Texture2D>
        , public ResourceStorage<Shader>
    {
    protected:
        struct Token
        {
        };

    public:
        static std::shared_ptr<ResourceManager> create();

        explicit ResourceManager(Token);
        ResourceManager() = delete;

        void addSearchPath(const std::string_view& searchPath, const std::string_view& mountPoint = "");
        void addSearchPaths(const std::map<std::string_view, std::string_view>& searchPaths);

        template <class T>
        void addFile(const std::string_view& name, const std::string_view& filename)
        {
            ResourceStorage<T>::m_files.try_emplace(name, std::vector { filename });
        }

        template <class T>
        void addFiles(const std::string_view& name, const std::vector<std::string_view>& filenames)
        {
            ResourceStorage<T>::m_files.try_emplace(name, filenames);
        }

        template <class T>
        void addFilesPack(const std::map<std::string_view, std::vector<std::string_view>>& files)
        {
            for (auto it : files)
            {
                ResourceStorage<T>::m_files.try_emplace(it.first, it.second);
            }
        }

        template <class T>
        void removeFiles(const std::string_view& name)
        {
            ResourceStorage<T>::m_files.erase(name);
        }

        template <class T>
        const std::shared_ptr<T>& load(const std::string_view& name)
        {
            auto& resources = ResourceStorage<T>::m_resources;
            if (!resources.empty())
            {
                const auto& it = resources.find(name);
                if (it != resources.end())
                {
                    return it->second;
                }
            }

            std::vector<std::string_view> filenames = ResourceStorage<T>::getFiles(name);
            if (filenames.empty())
            {
                constexpr std::string_view message = "Resource Load Error [%s]: can not find files";
                throw std::runtime_error(strings::format(message, name.data()));
            }

            const auto& pair = resources.emplace(name, T::createFromData(getData(filenames)));
            if (!pair.second)
            {
                constexpr std::string_view message = "Resource Load Error [%s]: can not save resource";
                throw std::runtime_error(strings::format(message, name.data()));
            }
            return pair.first->second;
        }

        template <class T, class... Args>
        const std::shared_ptr<T> loadOrCreate(const std::string_view& name, Args... args)
        {
            auto& resources = ResourceStorage<T>::m_resources;
            if (!resources.empty())
            {
                const auto& it = resources.find(name);
                if (it != resources.end())
                {
                    return it->second;
                }
            }

            std::shared_ptr<T> resource = T::create(args...);
            if (resource == nullptr)
            {
                return nullptr;
            }

            const auto& pair = resources.emplace(name, resource);
            if (!pair.second)
            {
                constexpr std::string_view message = "Resource Load Error [%s]: can not save resource";
                throw std::runtime_error(strings::format(message, name.data()));
            }
            return pair.first->second;
        }

        template <class T>
        bool unload(const std::string_view& name)
        {
            return ResourceStorage<T>::m_resources.erase(name) == 1;
        }

        template <class T>
        bool has(const std::string_view& name)
        {
            auto& resources = ResourceStorage<T>::m_resources;
            if (!resources.empty())
            {
                const auto& it = resources.find(name);
                return it != resources.end();
            }
            return false;
        }

    private:
        static std::string readFileData(const std::string_view& filename);

        std::map<std::string, std::string> getData(std::vector<std::string_view> filenames)
        {
            std::map<std::string, std::string> result;

            for (auto filename : filenames)
            {
                const std::string data = readFileData(filename);
                if (!data.empty())
                {
                    result.emplace(filename, data);
                }
            }

            return result;
        }
    };
}