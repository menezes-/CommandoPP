#pragma once
#include "GameEvent.hpp"
#include "tmx/MapObject.h"

class MObjectInView final: public GameEvent {
public:
    MObjectInView(const tmx::MapObject *mapObject, bool inView);

    const tmx::MapObject * getMapObject() const;

    bool isInView() const;

private:
    const tmx::MapObject* mapObject{nullptr};
    // true se o objeto apareceu, false se desapareceu
    bool inView;

};
