#include "../include/Entity.hpp"
#include <algorithm>
#include <cstdlib>
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
    }
    else if (state == DYING && isStopped()) {
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
        }
        else {
            --lives;
        }
    }

}
Entity::Entity(cgf::Game *gameObj, EntityType type, EntityConfig config)
    : gameObj(gameObj), type(type), config(config), health{config.health}, lives{config.lives} {}
