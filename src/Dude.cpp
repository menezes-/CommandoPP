#include <GameMath.hpp>
#include "Dude.hpp"
#include "events/FireEvent.hpp"


Dude::Dude(EntityConfig config, EventDispatcher &eventDispatcher)
    : Entity(DUDE, config, eventDispatcher), weapon{this, eventDispatcher, Weapons::SHOTGUN} {

    loadSmallSprites();
    loadAnimation("resources/animations/dude.xml");

    directionAnimation = {{Dir(0, 0), "Up"}, //default
                          {Dir(0, -1), "Up"},
                          {Dir(1, -1), "RightUp"},
                          {Dir(1, 0), "Right"},
                          {Dir(1, 1), "RightDown"},
                          {Dir(0, 1), "Down"},
                          {Dir(-1, 1), "LeftDown"},
                          {Dir(-1, 0), "Left"},
                          {Dir(-1, -1), "LeftUp"}};

    currAnimation = directionAnimation[currDir];
    setAnimation(currAnimation);
    setAnimRate(15);
    setState(ALIVE);

}


Dude::Dude(EventDispatcher &eventDispatcher)
    : Dude(EntityConfig{1, 150, false, true}, eventDispatcher) {

}


void Dude::setEnemy(Entity *enemy) {
    Dude::enemy = enemy;
}


void Dude::onEntityCollision(Entity &entity) {

}


void
Dude::onMapCollision(tmx::MapObject *mapObject, const sf::FloatRect &overlap, const sf::Vector2f &collisionNormal) {

    auto manifold = getManifold(overlap, collisionNormal);

    auto offset = sf::Vector2f(manifold.x, manifold.y) * manifold.z;
    move(offset);

}


void Dude::setPanic(bool panic) {
    Dude::panic = panic;
}


void Dude::setWeapon(Weapons weapon) {
    this->weapon.setWeapon(weapon);

}


void Dude::setError(float error) {
    Dude::error = error;
}


void Dude::update(cgf::Game *gameObj) {
    if (state == EntityState::ALIVE) {
        if (enemy) {
            if (!panicMode) {
                if (weapon.fire()) {
                    eventDispatcher
                        .notify(make_event<FireEvent>(getPosition(), enemy, weapon.getCurrWeaponConfig(), error));
                }
            }
        }
    }


    Entity::update(gameObj);
}


void Dude::die() {
    setXspeed(0);
    setYspeed(0);
    setAndPlay("Die");
    setAnimRate(2);
    Entity::die();
}
