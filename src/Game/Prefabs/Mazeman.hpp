#pragma once

#include <glm/vec2.hpp>

#include <memory>

namespace inari {
class EntityRegistry;
class Texture2D;
}  // namespace inari

namespace prefabs {
void createMazeman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                   const std::shared_ptr<inari::Texture2D>& texture,
                   const glm::vec2& position,
                   float angle);
}  // namespace prefabs