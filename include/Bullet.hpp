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

    void reuseBullet(EntityType owner, const WeaponConfig &weaponConfig, const sf::Vector2f &direction);

    void onMapCollision(
        tmx::MapObject *mapObject,
        const sf::FloatRect &overlap,
        const sf::Vector2f &collisionNormal

    ) override;

    void die();

private:
    int damage;
    EntityType owner;
    sf::Time lifetime;
    sf::Clock clock;
    bool init_clock{false};
};
