#pragma once

#include "Entity.hpp"
#include <map>

class Joe final: public Entity {
public:
    Joe(cgf::Game *gameObj, EntityConfig config, EventDispatcher &eventDispatcher);

    Joe(cgf::Game *gameObj, EventDispatcher &eventDispatcher);

    void onEntityCollision(Entity &entity) override;

    void handleInput(const KeyBitset &pressedKeys) override;

    void update() override;

private:
    std::map<std::pair<int, int>, std::string> directions;

};


