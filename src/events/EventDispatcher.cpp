#include "events/EventDispatcher.hpp"


void EventDispatcher::addObserver(Event event, std::unique_ptr<Observer> ptr) {

    auto &ins = observers[event];
    ptr->setEventDispatcher(this);
    ins.push_back(std::move(ptr));

}

void EventDispatcher::notify(const std::shared_ptr<GameEvent>& event) {
    auto search = observers.find(event->getEvent());
    if (search == observers.end()) {
        return;
    }

    for (auto &observer: search->second) {
        observer->onNotify(event);
    }
}


EventDispatcher::EventDispatcher()
    : observers{} {}
