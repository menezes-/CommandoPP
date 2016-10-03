#include "../include/EventDispatcher.hpp"
void EventDispatcher::addObserver(GameEvent event, ObserverPtr ptr) {

    auto &ins = observers[event];
    ins.push_back(std::move(ptr));

}
void EventDispatcher::notify(const Entity &entity, GameEvent event) {
    auto search = observers.find(event);
    if (search == observers.end()) {
        return;
    }

    for (auto &observer: search->second) {
        observer->onNotify(entity, event);
    }

}
EventDispatcher::EventDispatcher()
    : observers{} {}
