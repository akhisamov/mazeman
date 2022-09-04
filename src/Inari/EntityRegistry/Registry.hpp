#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

namespace inari
{
    struct Entity
    {
        std::string uuid;
        std::string name;
    };
    using EntityPtr = std::shared_ptr<Entity>;

    class Registry
    {
        using ComponentHash = size_t;
        using AnyComponent = std::any;
        using ComponentMap = std::map<ComponentHash, AnyComponent>;

    public:
        Registry() = default;
        ~Registry() = default;

        Registry(Registry&&) = delete;
        Registry(const Registry&) = delete;

        const EntityPtr& create(const std::string_view& name = "");
        bool destroy(const EntityPtr& entity);
        bool destroyByName(const std::string_view& name);

        template <class C, class... Args>
        bool emplace(const EntityPtr& entity, Args... args)
        {
            auto& entityIt = m_collection.find(entity);
            if (entityIt != m_collection.end())
            {
                ComponentHash hashCode = typeid(C).hash_code();
                auto& componentMap = entityIt->second;
                if (componentMap.find(hashCode) == componentMap.end())
                {
                    componentMap.emplace(hashCode, std::any(C(args...)));
                }
            }
            return false;
        }

        template <class C>
        C* get(const EntityPtr& entity)
        {
            auto& entityIt = m_collection.find(entity);
            if (entityIt != m_collection.end())
            {
                auto& componentIt = entityIt->second.find(typeid(C).hash_code());
                if (componentIt != entityIt->second.end())
                {
                    return std::any_cast<C>(&componentIt->second);
                }
            }

            return nullptr;
        }

    private:
        std::map<EntityPtr, ComponentMap> m_collection;
    };
}