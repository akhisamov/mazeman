#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

#include "Inari/ECS/Systems/ISystem.h"

namespace inari {
    using EntityRegistryPtr = std::shared_ptr<class EntityRegistry>;
    class GameTime;

    class SystemRegistry {
        using SystemHash = std::size_t;

    public:
        SystemRegistry() = default;
        ~SystemRegistry() = default;

        SystemRegistry(SystemRegistry&&) = delete;
        SystemRegistry(const SystemRegistry&) = delete;

        template <class S, class... Args>
        std::shared_ptr<S> addSystem(Args... args)
        {
            static_assert(std::is_base_of_v<ISystem, S>, "Wrong System Type");
            const SystemHash hashCode = typeid(S).hash_code();
            if (m_systems.find(hashCode) == m_systems.end()) {
                auto system = std::make_shared<S>(args...);
                auto resultPair = m_systems.emplace(hashCode, system);
                if (resultPair.second) {
                    return system;
                }
            }
            return nullptr;
        }

        template <class S, class... Args>
        std::shared_ptr<S> addOrderedSystem(Args... args)
        {
            auto system = addSystem<S>(args...);
            if (system) {
                const SystemHash hashCode = typeid(S).hash_code();
                m_order.push_back(hashCode);
            }
            return system;
        }

        template <class S>
        std::shared_ptr<S> getSystem() const
        {
            static_assert(std::is_base_of_v<ISystem, S>, "Wrong System Type");
            const auto& it = m_systems.find(typeid(S).hash_code());
            if (it != m_systems.end()) {
                return std::dynamic_pointer_cast<S>(it->second);
            }
            return nullptr;
        }

        template <class S>
        void updateSystem(const GameTime& gameTime, const EntityRegistryPtr& entityRegistry)
        {
            static_assert(std::is_base_of_v<ISystem, S>, "Wrong System Type");
            const auto& it = m_systems.find(typeid(S).hash_code());
            if (it != m_systems.end()) {
                updateSystem(it->second, gameTime, entityRegistry);
            }
        }

        void updateOrderedSystem(const GameTime& gameTime, const EntityRegistryPtr& entityRegistry);
        void updateSystem(const std::shared_ptr<ISystem>& system, const GameTime& gameTime,
                          const EntityRegistryPtr& entityRegistry);

    private:
        std::vector<SystemHash> m_order;
        std::map<SystemHash, std::shared_ptr<ISystem>> m_systems;
    };
} // namespace inari
