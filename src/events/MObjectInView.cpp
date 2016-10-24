#include "events/MObjectInView.hpp"


MObjectInView::MObjectInView(const tmx::MapObject *mapObject)
    : GameEvent(Event::MAP_OBJECT_IN_VIEW), mapObject(mapObject) {}


const tmx::MapObject * MObjectInView::getMapObject() const {
    return mapObject;
}
