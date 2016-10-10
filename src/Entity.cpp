#include "Entity.hpp"
#include <Debug.h>
#include "Bullet.hpp"

int Entity::s_id;


Entity::Entity(EntityType type,
               EntityConfig config,
               EventDispatcher &eventDispatcher)
    : type{type}, config(config), health{config.health}, lives{config.lives},
      eventDispatcher{eventDispatcher}, id{++s_id} {}


const EntityConfig &Entity::getConfig() const {
    return config;
}


void Entity::setConfig(EntityConfig config) {
    Entity::config = config;
}


void Entity::update(cgf::Game *gameObj) {
    if (!isVisible()) return;

    switch (state) {
        case DEAD:
            if (isVisible()) {
                setVisible(false);
            }
            eventDispatcher.notify(make_event<GameEvent>(this, ENTITY_IS_DEAD));
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
                state = DEAD;
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
        eventDispatcher.notify(make_event<GameEvent>(this, Event::ENTITY_IS_DYING));
        health = config.health;

    } else {
        eventDispatcher.notify(make_event<GameEvent>(this, Event::ENTITY_TOOK_DAMAGE));
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


void Entity::loadSmallSprites() {
    load("resources/sprites/sprites_small.png", 24, 24, 0, 0, 0, 0, 15, 9);

}


bool Entity::isEnemy(const Entity &other) {
    auto ot = other.type;
    if (other.type == BULLET) {
        auto bullet = static_cast<const Bullet &>(other);
        ot = bullet.getOwner();
    }

    if (type == JOE && ot != JOE) {
        return true;
    } else return type != JOE && ot == JOE;

}


int Entity::getId() {
    return id;
}


Entity::operator std::string() const {
    std::string type_name{};
    switch (type) {
        case JOE:
            type_name = "Joe";
            break;
        case DUDE:
            type_name = "Dude";
            break;
        case BULLET:
            type_name = "Bullet";
            break;
    }

    return type_name + std::string("#") + std::to_string(id);

}


EntityConfig::EntityConfig(int lives, int health, bool godMode, bool movable)
    : lives(lives), health(health), godMode(godMode), movable(movable) {}
