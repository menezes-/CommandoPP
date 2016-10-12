#include <events/CollisionEvent.hpp>
#include "systems/CollisionSystem.hpp"


void CollisionSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    if(event->getEvent() != Event::COLLISION_EVENT) return;

    const CollisionEvent *fire_event = static_cast<const CollisionEvent *>(event.get());

    if(Entity* target = fire_event->getTarget()){
        target->onEntityCollision(*fire_event->getFrom());
        fire_event->getFrom()->onEntityCollision(*target);
    } else if (tmx::MapObject* mapObject = fire_event->getMapObject()){
        fire_event->getFrom()->onMapCollision(mapObject);
    }



}
