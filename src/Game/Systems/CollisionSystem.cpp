#include "CollisionSystem.hpp"

#include "Game/Components/Collision.hpp"

CollisionSystem::CollisionSystem(
    std::shared_ptr<inari::EntityRegistry> registry)
    : ISystem(std::move(registry)) {}

void CollisionSystem::update(float dt, const inari::EntityPtr& entity) {
    // TODO
    const auto* collision = getRegistry()->getComponent<Collision>(entity);
    if (collision == nullptr || !collision->isDynamic) {
        return;
    }
}