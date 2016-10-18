#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"
#include <map>

class Joe final: public Entity {
public:
    Joe(EntityConfig config, EventDispatcher &eventDispatcher);

    Joe(EventDispatcher &eventDispatcher);

    void onEntityCollision(Entity &entity) override;

    void onMapCollision(
        tmx::MapObject *mapObject,
        const sf::FloatRect &overlap,
        const sf::Vector2f &collisionNormal

    ) override;

    void handleInput(const KeyBitset &pressedKeys, const MouseBitset &pressedButtons, cgf::Game *gameObj);

    Weapon &getWeapon();

    void die() override;

private:
    std::map<Dir, std::string> directionAnimation;
    Weapon weapon;
};


