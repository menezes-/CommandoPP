#include "Bullet.hpp"

Bullet::Bullet(EventDispatcher &eventDispatcher,
               EntityType owner,
               const WeaponConfig &weaponConfig,
               const sf::Vector2f &direction)
    : Entity(BULLET, EntityConfig(1, 1, weaponConfig.destroyable, true ), eventDispatcher) {
    damage = weaponConfig.ammo_damage;
    lifetime = weaponConfig.ammo_lifetime;
    this->owner = owner;

    setXspeed(direction.x * weaponConfig.ammo_velocity);
    setYspeed(direction.y * weaponConfig.ammo_velocity);

    loadSmallSprites();
    setFrameRange(129, 132);

}


EntityType Bullet::getOwner() const {
    return owner;
}


void Bullet::update(cgf::Game *gameObj) {
    static bool init_clock = false;

    sf::Time elapsed;
    if (!init_clock) {
        elapsed = clock.restart();
        init_clock = true;
    } else {
        elapsed = clock.getElapsedTime();
    }

    if (state == ALIVE) {
        if (elapsed >= lifetime) {
            state = DYING;
            eventDispatcher.notify(make_event<GameEvent>(this, Event::ENTITY_IS_DYING));
            play();
            setXspeed(0);
            setYspeed(0);
        }


    }
    Entity::update(gameObj);
}


void Bullet::onEntityCollision(Entity &other) {

    if (isEnemy(other)) {
        if (!other.getConfig().godMode) {
            other.loseHealth(damage);
            eventDispatcher.notify(make_event<GameEvent>(this, &other, ENTITY_SHOT_ENTITY));
        }
    }

}
