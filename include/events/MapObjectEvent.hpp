#pragma once
#include "GameEvent.hpp"
#include "tmx/MapObject.h"

class MapObjectEvent final: GameEvent {
public:
    MapObjectEvent(tmx::MapObject *mapObject);

    tmx::MapObject *getMapObject() const;

private:
    tmx::MapObject* mapObject{nullptr};

};
