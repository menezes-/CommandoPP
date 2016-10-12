#include "events/CollisionEvent.hpp"


CollisionEvent::CollisionEvent(Entity *from, Entity *target)
    : GameEvent(from, target, Event::COLLISION_EVENT) {}


CollisionEvent::CollisionEvent(Entity *from, tmx::MapObject *mapObject)
    : GameEvent(from, Event::COLLISION_EVENT), mapObject{mapObject} {

}


tmx::MapObject * CollisionEvent::getMapObject() const {
    return mapObject;
}
