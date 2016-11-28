#include "events/MObjectInView.hpp"


MObjectInView::MObjectInView(const tmx::MapObject *mapObject, bool inView)
    : GameEvent(Event::MAP_OBJECT_IN_VIEW), mapObject(mapObject), inView{inView} {}


const tmx::MapObject * MObjectInView::getMapObject() const {
    return mapObject;
}


bool MObjectInView::isInView() const {
    return inView;
}
