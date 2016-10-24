#include "events/MapObjectEvent.hpp"


MapObjectEvent::MapObjectEvent(tmx::MapObject *mapObject)
    : GameEvent(Event::MAP_OBJECT_IN_VIEW), mapObject(mapObject) {}


tmx::MapObject *MapObjectEvent::getMapObject() const {
    return mapObject;
}
