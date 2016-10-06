#pragma once
#include <map>
#include <vector>
#include "Observer.hpp"


class EventDispatcher {
public:
    void addObserver(GameEvent event, ObserverPtr ptr);

    void notify(const Entity *entity, GameEvent event);

    EventDispatcher();

private:
    std::map<GameEvent, std::vector<ObserverPtr>> observers;

};


