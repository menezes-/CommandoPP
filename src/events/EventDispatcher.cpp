#include "events/EventDispatcher.hpp"


void EventDispatcher::notify(std::shared_ptr<GameEvent> event) {
    auto event1 = event->getEvent();

    auto search = observersMap.find(event1);
    if (search == observersMap.end()) {
        return;
    }

    for (auto listener: search->second) {
        eventQueue.emplace_back(listener, event);
    }

}


EventDispatcher::EventDispatcher()
    : observersMap{} {}


void EventDispatcher::dispatchEvents() {

    while (!eventQueue.empty()) {
        auto ep = eventQueue.front();
        ep.first->onNotify(ep.second);
        eventQueue.pop_front();

    }

}
