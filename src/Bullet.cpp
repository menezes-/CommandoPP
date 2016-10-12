#include "Bullet.hpp"


Bullet::Bullet(EventDispatcher &eventDispatcher,
               EntityType owner,
               const WeaponConfig &weaponConfig,
               const sf::Vector2f &direction)
    : Entity(BULLET, EntityConfig(1, 1, weaponConfig.destroyable, true), eventDispatcher) {
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
    if (state == ALIVE) {
        sf::Time elapsed;
        if (!init_clock) {
            elapsed = clock.restart();
            init_clock = true;
        } else {
            elapsed = clock.getElapsedTime();
        }


        if (elapsed >= lifetime) {
            die();
        }

    }
    Entity::update(gameObj);
}


void Bullet::die() {
    if(state == ALIVE) {
        setXspeed(0);
        setYspeed(0);
        loseHealth(1000);
        play();
        setLooped(false);
    }
}


void Bullet::onEntityCollision(Entity &other) {

    if (isEnemy(other)) {
        if (!other.getConfig().godMode) {
            other.loseHealth(damage);
            eventDispatcher.notify(make_event<GameEvent>(this, &other, ENTITY_SHOT_ENTITY));
        }
    }

}


void Bullet::onMapCollision(tmx::MapObject *mapObject) {

    auto vh_prop = mapObject->GetPropertyString("vh");
    // se meu objeto não é "vh"(very high) e a bala vem "de trás" do objeto
    // então nenhuma colisão aconteceu (atirando de cobertura)
    if (vh_prop.empty() && getYspeed() > 0) {
        return;
    } else {
        die();
    }

}
