#include "../include/Entity.hpp"


Entity::Entity(cgf::Game *gameObj,
               EntityType type,
               EntityConfig config,
               EventDispatcher &eventDispatcher)
    : gameObj{gameObj}, type{type}, config(config), health{config.health}, lives{config.lives},
      eventDispatcher{eventDispatcher} {}


const EntityConfig &Entity::getConfig() const {
    return config;
}


void Entity::setConfig(EntityConfig config) {
    Entity::config = config;
}


void Entity::update() {
    if (state != DEAD) {
        auto delta = gameObj->getUpdateInterval();
        cgf::Sprite::update(delta, config.movable);
    } else if (state == DYING && isStopped()) {
        state = DEAD;
        setVisible(false);
    }

}


void Entity::loseHealth(int amount) {
    if (config.godMode || state == DEAD || state == DYING) {
        return;
    }
    amount = std::abs(amount);

    health = std::max(0, health - amount);

    if (health <= 0) {
        if (lives <= 1) {
            state = DYING;
            eventDispatcher.notify(*this, ENTITY_IS_DEAD);
        } else {
            --lives;
            eventDispatcher.notify(*this, GameEvent::ENTITY_LOST_LIFE);
            health = config.health;
        }
    } else {
        eventDispatcher.notify(*this, GameEvent::ENTITY_TOOK_DAMAGE);
    }

}


EntityType Entity::getType() const {
    return type;
}


EntityState Entity::getState() const {
    return state;
}


int Entity::getLives() const {
    return lives;
}


int Entity::getHealth() const {
    return health;
}
