#pragma once
#include <deque>
#include <unordered_map>
#include <vector>
#include "Event.hpp"
#include "Observer.hpp"


class EventDispatcher {
public:

    template<typename... Events>
    void addObserver(Observer *ptr, Events...events) {
        ptr->setEventDispatcher(this);
        auto e_list = std::initializer_list<Event>{events...};
        for (auto event: e_list) {
            auto &ins = observersMap[event];
            ins.push_back(ptr);
        }

    }

    void notify(std::shared_ptr<GameEvent> event);

    void dispatchEvents();

    EventDispatcher();

private:
    std::unordered_map<Event, std::vector<Observer *>> observersMap;
    std::deque<std::pair<Observer *, std::shared_ptr<GameEvent>>> eventQueue;

};


