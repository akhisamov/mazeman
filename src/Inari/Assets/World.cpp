#include "World.h"

namespace {
    constexpr std::string_view levelPrefix = "Level_";
}

namespace inari {
    World::World(std::unique_ptr<Data>&& data)
        : m_data(std::move(data))
    {
    }
    World::~World() = default;

    const WorldLevel& World::getLevel(int idx) const
    {
        const auto it = m_data->levels.find(levelPrefix.data() + std::to_string(idx));
        assert(it != m_data->levels.end());
        return it->second;
    }

} // namespace inari