#pragma once

#include <cassert>
#include <map>
#include <memory>

#include "Inari/ECS/Systems/ISystem.hpp"

namespace inari {
class EntityRegistry;

class SystemRegistry {
   public:
    SystemRegistry() = default;
    ~SystemRegistry() = default;

    SystemRegistry(SystemRegistry&&) = delete;
    SystemRegistry(const SystemRegistry&) = delete;

    template <class S, class... Args>
    std::shared_ptr<S> addSystem(
        const std::shared_ptr<EntityRegistry>& entityRegistry,
        Args... args) {
        assert(entityRegistry != nullptr && "Entity registry is empty");

        const size_t hashCode = typeid(S).hash_code();
        if (m_systems.find(hashCode) == m_systems.end()) {
            auto system = std::make_shared<S>(entityRegistry, args...);
            auto resultPair = m_systems.emplace(hashCode, system);
            if (resultPair.second) {
                return system;
            }
        }

        return nullptr;
    }

    template <class S>
    std::shared_ptr<S> getSystem() {
        auto it = m_systems.find(typeid(S).hash_code());
        if (it != m_systems.end()) {
            return std::dynamic_pointer_cast<S>(it->second);
        }
        return nullptr;
    }

   private:
    std::map<size_t, std::shared_ptr<ISystem>> m_systems;
};
}  // namespace inari