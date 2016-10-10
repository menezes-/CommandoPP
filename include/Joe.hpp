#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"
#include <map>

class Joe final: public Entity {
public:
    Joe(EntityConfig config, EventDispatcher &eventDispatcher);

    Joe(EventDispatcher &eventDispatcher);

    void onEntityCollision(Entity &entity) override;

    void handleInput(const KeyBitset &pressedKeys, const MouseBitset &pressedButtons, cgf::Game * gameObj);

    void update(cgf::Game *gameObj) override;

    Weapon & getWeapon();

private:
    std::map<Dir, std::string> directions;
    Weapon weapon;
};


