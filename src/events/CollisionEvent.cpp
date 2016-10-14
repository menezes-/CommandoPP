#include "events/CollisionEvent.hpp"


CollisionEvent::CollisionEvent(Entity *from, Entity *target)
    : GameEvent(from, target, Event::COLLISION_EVENT) {}


CollisionEvent::CollisionEvent(Entity *from, tmx::MapObject *mapObject)
    : GameEvent(from, Event::COLLISION_EVENT), mapObject{mapObject} {

}


CollisionEvent::CollisionEvent(Entity *from,
                               tmx::MapObject *mapObject,
                               sf::FloatRect overlap,
                               sf::Vector2f collisionNormal)
    : GameEvent(from, Event::COLLISION_EVENT), mapObject{mapObject}, overlap{overlap},
      collisionNormal{collisionNormal} {

}


tmx::MapObject *CollisionEvent::getMapObject() const {
    return mapObject;
}


const sf::FloatRect &CollisionEvent::getOverlap() const {
    return overlap;
}


const sf::Vector2f &CollisionEvent::getCollisionNormal() const {
    return collisionNormal;
}
