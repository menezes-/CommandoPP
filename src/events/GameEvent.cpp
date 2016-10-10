#include "events/GameEvent.hpp"


GameEvent::GameEvent(Entity *from, Entity *target, Event event)
    : from{from}, target{target}, event{event} {

}


GameEvent::GameEvent(Entity *from, Event event)
    : GameEvent(from, nullptr, event) {

}


GameEvent::GameEvent(Event event)
    : GameEvent(nullptr, event) {

}


Event GameEvent::getEvent() const {
    return event;
}


Entity *GameEvent::getFrom() const {
    return from;
}


Entity *GameEvent::getTarget() const {
    return target;
}
