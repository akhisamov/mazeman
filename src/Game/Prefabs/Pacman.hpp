#pragma once

#include <memory>

namespace inari {
class EntityRegistry;
class Texture2D;
}  // namespace inari

namespace prefabs {
void createPacman(const std::shared_ptr<inari::EntityRegistry>& entityRegistry,
                  const std::shared_ptr<inari::Texture2D>& texture);
}  // namespace prefabs