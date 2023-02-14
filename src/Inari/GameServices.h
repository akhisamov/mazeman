#pragma once

#include <map>
#include <memory>

namespace inari {
    class IService;

    class GameServices {
    public:
        template <class T>
        static std::shared_ptr<T> get()
        {
            static_assert(std::is_base_of_v<IService, T>);
            const size_t hash = typeid(T).hash_code();
            const auto& it = m_services.find(hash);
            if (it != m_services.end()) {
                return std::dynamic_pointer_cast<T>(it->second.lock());
            }
            return nullptr;
        }

        template <class T>
        static bool has()
        {
            static_assert(std::is_base_of_v<IService, T>);
            const size_t hash = typeid(T).hash_code();
            return m_services.find(hash) != m_services.end();
        }

        template <class T>
        static void provide(const std::shared_ptr<T>& service)
        {
            static_assert(std::is_base_of_v<IService, T>);
            const size_t hash = typeid(T).hash_code();
            assert(m_services.find(hash) == m_services.end());
            m_services.emplace(hash, service);
        }

        template <class T>
        static void remove()
        {
            static_assert(std::is_base_of_v<IService, T>);
            const size_t hash = typeid(T).hash_code();
            const auto& it = m_services.find(hash);
            assert(it != m_services.end());
            m_services.erase(it);
        }

    private:
        static std::map<std::size_t, std::weak_ptr<IService>> m_services;
    };
}