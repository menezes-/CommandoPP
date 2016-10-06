#pragma once

#include "Entity.hpp"
#include <memory>

enum GameEvent {
    ENTITY_IS_DEAD,
    ENTITY_LOST_LIFE,
    ENTITY_TOOK_DAMAGE,
};

class Entity;
class Observer {
public:
    virtual ~Observer() {}

    virtual void onNotify(const Entity *entity, GameEvent event) =0;

};
using ObserverPtr = std::unique_ptr<Observer>;
