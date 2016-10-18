#include "Entity.hpp"
#include <Debug.h>
#include "Bullet.hpp"

std::size_t Entity::s_id;


Entity::Entity(EntityType type,
               EntityConfig config,
               EventDispatcher &eventDispatcher)
    : type{type}, config(config), health{config.health}, lives{config.lives},
      eventDispatcher{eventDispatcher}, id{s_id++} {}


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
                eventDispatcher.notify(make_event<GameEvent>(this, ENTITY_IS_DEAD));
                return; // vou para o estado de morto e termino o update
            }
        default: // se eu não estou no estado "DYING" OU eu ESTOU no estado "DYING" E a animação de morte NÃO terminou
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
        if (lives > 0) {
            health = config.health;
        }
        die();
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


bool Entity::isEnemy(const Entity &other) const {
    auto ot = other.type;
    auto myt = type;
    if (myt == BULLET) {
        auto bptr = static_cast<const Bullet *>(this);
        myt = bptr->getOwner();
    }
    if (other.type == BULLET) {
        auto bullet = static_cast<const Bullet &>(other);
        ot = bullet.getOwner();
    }

    if (myt == JOE && ot != JOE) {
        return true;
    } else if (myt == DUDE && ot != DUDE) {
        return true;
    }
    return false;

}


std::size_t Entity::getId() const {
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


sf::FloatRect Entity::getBoundingBox() {
    auto size = getSize();
    auto spriteW = size.x;
    auto spriteH = size.y;
    sf::Vector2f pos = this->getPosition();
    sf::Vector2f scale = this->getScale();

    auto width = spriteW / 2 * scale.x;

    auto height = spriteH / 2 * scale.y;
    auto x0 = pos.x - width;
    auto y1 = pos.y + height;
    sf::FloatRect floatRect{x0, y1, width, height};
    return floatRect;
}


void Entity::setState(EntityState state) {
    Entity::state = state;
}


void Entity::die() {
    state = DYING;
    eventDispatcher.notify(make_event<GameEvent>(this, Event::ENTITY_IS_DYING));
}


EntityConfig::EntityConfig(int lives, int health, bool godMode, bool movable)
    : lives(lives), health(health), godMode(godMode), movable(movable) {}
