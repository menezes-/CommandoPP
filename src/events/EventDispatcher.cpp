#include "events/EventDispatcher.hpp"


void EventDispatcher::notify(std::shared_ptr<GameEvent> event) {
    auto event1 = event->getEvent();
    if (observersMap.empty())
        return;
    auto search = observersMap.find(event1);
    if (search == observersMap.end()) {
        return;
    }

    for (auto observer: search->second) {
        observer->onNotify(event);
    }
}


EventDispatcher::EventDispatcher()
    : observers{}, observersMap{} {}
