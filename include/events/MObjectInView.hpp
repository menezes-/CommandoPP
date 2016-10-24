#pragma once
#include "GameEvent.hpp"
#include "tmx/MapObject.h"

class MObjectInView final: public GameEvent {
public:
    MObjectInView(const tmx::MapObject *mapObject);

    const tmx::MapObject * getMapObject() const;

private:
    const tmx::MapObject* mapObject{nullptr};

};
