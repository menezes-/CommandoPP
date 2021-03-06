#include "Bullet.hpp"


Bullet::Bullet(EventDispatcher &eventDispatcher,
               EntityType owner,
               const WeaponConfig &weaponConfig,
               const sf::Vector2f &direction)
    : Entity(BULLET, EntityConfig(1, 1, weaponConfig.destroyable, true), eventDispatcher) {

    loadSmallSprites();
    reuseBullet(owner, weaponConfig, direction);
    //state = EntityState::DEAD;

}


EntityType Bullet::getOwner() const {
    return owner;
}


void Bullet::update(cgf::Game *gameObj) {
    if (state == ALIVE) {
        sf::Time elapsed;
        if (!init_clock) {
            clock.restart();
            init_clock = true;
        }
        elapsed = clock.getElapsedTime();


        if (elapsed >= lifetime) {
            die();
        }

    }
    Entity::update(gameObj);
}


void Bullet::die() {
    if (state == ALIVE) {
        setXspeed(0);
        setYspeed(0);
        play();
        setLooped(false);
        Entity::die();
    }
}


void Bullet::onEntityCollision(Entity &other) {

    if (isEnemy(other)) {
        if (!other.getConfig().godMode) {
            other.loseHealth(damage);
            die();
        }
    }

}


void Bullet::onMapCollision(
    tmx::MapObject *mapObject,
    const sf::FloatRect &overlap,
    const sf::Vector2f &collisionNormal

) {
    auto vh_prop = mapObject->GetPropertyString("vh");
    // se meu objeto não é "vh"(very high) e a bala vem "de trás" do objeto
    // então nenhuma colisão aconteceu (atirando de cobertura)
    if (vh_prop.empty() && getYspeed() > 0) {
        return;
    } else {
        die();
    }
}


void Bullet::reuseBullet(EntityType owner, const WeaponConfig &weaponConfig, const sf::Vector2f &direction) {

    damage = weaponConfig.ammo_damage;
    lifetime = weaponConfig.ammo_lifetime;
    this->owner = owner;

    setXspeed(direction.x * weaponConfig.ammo_velocity);
    setYspeed(direction.y * weaponConfig.ammo_velocity);
    state = EntityState::ALIVE;
    config.godMode = !weaponConfig.destroyable;
    init_clock = false;
    if (weaponConfig.destroyable) {
        setFrameRange(123, 128);
    } else {
        setFrameRange(129, 132);
    }
    setLooped(false);

}
