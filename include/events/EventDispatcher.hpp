#pragma once
#include <map>
#include <vector>
#include "Event.hpp"
#include "Observer.hpp"


class EventDispatcher {
public:
    void addObserver(Event event, std::unique_ptr<Observer> ptr);

    void notify(const std::shared_ptr<GameEvent>& event);

    EventDispatcher();

private:
    std::map<Event , std::vector<std::unique_ptr<Observer>>> observers;

};


