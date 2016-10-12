#pragma once
#include <SFML/System.hpp>
#include "Entity.hpp"
#include "Weapon.hpp"

class Bullet final: public Entity {
public:
    Bullet(EventDispatcher &eventDispatcher,
           EntityType owner,
           const WeaponConfig &weaponConfig,
           const sf::Vector2f &direction);

    EntityType getOwner() const;

    void update(cgf::Game *gameObj) override;

    void onEntityCollision(Entity &other) override;

    void onMapCollision(tmx::MapObject *mapObject) override;

private:
    int damage;
    EntityType owner;
    sf::Time lifetime;
    sf::Clock clock;

    void die();
};
