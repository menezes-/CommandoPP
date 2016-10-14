#pragma once
#include "GameEvent.hpp"
#include "tmx/MapObject.h"

class CollisionEvent final: public GameEvent {
public:
    CollisionEvent(Entity *from, Entity *target);

    CollisionEvent(Entity *from, tmx::MapObject *mapObject);

    CollisionEvent(Entity *from, tmx::MapObject *mapObject, sf::FloatRect overlap, sf::Vector2f collisionNormal);

    tmx::MapObject *getMapObject() const;

    const sf::FloatRect &getOverlap() const;

    const sf::Vector2f &getCollisionNormal() const;

private:
    tmx::MapObject *mapObject{nullptr};
    sf::FloatRect overlap;
    sf::Vector2f collisionNormal;

};
