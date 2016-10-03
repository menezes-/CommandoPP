#pragma once

#include "Entity.hpp"

class Joe: public Entity {
public:
    Joe(cgf::Game *gameObj, EntityConfig config, EventDispatcher &eventDispatcher);

    void onEntityCollision(Entity &entity) override;

};


