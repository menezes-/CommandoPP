#pragma once

#include <memory>
#include "EventDispatcher.hpp"
#include "GameEvent.hpp"

class EventDispatcher;
class GameEvent;

class Observer {
public:
    virtual ~Observer() {}

    virtual void onNotify(const std::shared_ptr<GameEvent>& event) =0;

    void setEventDispatcher(EventDispatcher *eventDispatcher) {
        this->eventDispatcher = eventDispatcher;
    }


protected:
    EventDispatcher* eventDispatcher;

};
