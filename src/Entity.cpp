#include "../include/Entity.hpp"
#include <Debug.h>


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
    if (!isVisible()) return;

    switch (state) {
        case DEAD:
            break;
        /*
         * Case Fallthroug.
         * Se a entidade está no estado dying, ou seja, a animação de morte ta "acontecendo"
         * eu ainda preciso dar um Sprite::update, porém se a animação já terminou eu preciso sair do
         * estado DYING e ir pro estado DEAD, por isso o uso do Case Fallthrough.
         *
         */
        case DYING:
            if (isStopped()) { // terminei de tocar a animação de morte ?
                DEBUG_MSG("Entidade Morreu");
                state = DEAD;
                setVisible(false);
                return; // vou para o estado de morto e termino o update
            }
        default: // se eu não estou no estado "DYING" OU eu ESTOU no estado "DYING" E a animção de morte NÃO terminou
            auto delta = gameObj->getUpdateInterval();
            cgf::Sprite::update(delta, config.movable);
            break;

    }

}


void Entity::loseHealth(int amount) {
    if (config.godMode || state == DEAD || state == DYING) {
        return;
    }
    amount = std::abs(amount);

    health = std::max(0, health - amount);
    DEBUG_MSG("Entidade perdeu " << amount << " de vida ficou com " << health);
    if (health <= 0) {
        --lives;
        state = DYING;
        if (lives <= 0) {
            eventDispatcher.notify(this, ENTITY_IS_DEAD);
        } else {
            eventDispatcher.notify(this, GameEvent::ENTITY_LOST_LIFE);
            health = config.health;
        }
    } else {
        eventDispatcher.notify(this, GameEvent::ENTITY_TOOK_DAMAGE);
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


void Entity::setAndPlay(std::string name) {
    if (name == currAnimation) {
        return;
    }

    if (!name.empty()) {
        currAnimation = name;
        setAnimation(name);
        play();
    }

}
