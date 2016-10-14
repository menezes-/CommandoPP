#include <events/CollisionEvent.hpp>
#include "systems/CollisionSystem.hpp"


void CollisionSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    if (event->getEvent() != Event::COLLISION_EVENT) return;

    const CollisionEvent *collision_event = static_cast<const CollisionEvent *>(event.get());

    if (Entity *target = collision_event->getTarget()) {
        target->onEntityCollision(*collision_event->getFrom());
        collision_event->getFrom()->onEntityCollision(*target);
    } else if (collision_event->getMapObject()) {
        collision_event->getFrom()
            ->onMapCollision(collision_event->getMapObject(), collision_event->getOverlap(), collision_event->getCollisionNormal());
    }

}
