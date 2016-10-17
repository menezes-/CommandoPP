#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include "Event.hpp"
#include "Observer.hpp"


class EventDispatcher {
public:

    template<typename... Events>
    void addObserver(std::unique_ptr<Observer> ptr, Events...events) {
        ptr->setEventDispatcher(this);
        auto e_list = std::initializer_list<Event >{events...};
        auto n_ptr = ptr.get();
        for(auto event: e_list){
            auto &ins = observersMap[event];
            ins.push_back(n_ptr);
        }
        observers.push_back(std::move(ptr));

    }

    void notify(std::shared_ptr<GameEvent> event);

    EventDispatcher();

private:
    std::vector<std::unique_ptr<Observer>> observers;
    std::unordered_map<int, std::vector<Observer *>> observersMap;


};


