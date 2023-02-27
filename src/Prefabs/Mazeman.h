#pragma once

#include <Inari/Common/VecTypes.h>

#include <memory>

namespace inari {
    class EntityRegistry;
    class Texture2D;
} // namespace inari

namespace prefabs {
    void createMazeman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                       const std::shared_ptr<inari::Texture2D>& texture, const inari::Vec2f& position, float angle);
} // namespace prefabs