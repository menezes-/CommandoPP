#pragma once
#include "Entity.hpp"

class Dude final: Entity {
public:
    Dude(EntityConfig config, EventDispatcher &eventDispatcher);

    Dude(EventDispatcher &eventDispatcher);

private:
    std::map<Dir, std::string> directions;

};
