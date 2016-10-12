#pragma once
#include "GameEvent.hpp"
#include "tmx/MapObject.h"

class CollisionEvent final: public GameEvent {
public:
    CollisionEvent(Entity *from, Entity *target);

    CollisionEvent(Entity *from, tmx::MapObject* mapObject);

    tmx::MapObject * getMapObject() const;

private:
    tmx::MapObject* mapObject{nullptr};


};
