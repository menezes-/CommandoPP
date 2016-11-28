#include <Joe.hpp>
#include "events/FireEvent.hpp"
#include "GameMath.hpp"


sf::Vector2f FireEvent::getDirection() const {
    return direction;
}


sf::Vector2f FireEvent::getPosition() const {
    return position;
}


const WeaponConfig &FireEvent::getWeaponConfig() const {
    return weaponConfig;
}


FireEvent::FireEvent(Entity *from, sf::Vector2f direction)
    : GameEvent(from, FIRE) {

    this->direction = direction;
    position = from->getPosition();

    if (from->getType() == JOE) {
        Joe *joe = static_cast<Joe *>(from);
        auto weaponC = joe->getWeapon();
        weaponConfig = *weaponC.getCurrWeaponConfig();
    }

}


FireEvent::FireEvent(sf::Vector2f from, Entity *target, const WeaponConfig *weaponConfig, float error)
    : GameEvent(FIRE) {

    auto angle = interceptionAngle(from,
                                   target->getPosition(),
                                   sf::Vector2f{target->getXspeed(), target->getYspeed()},
                                   weaponConfig->ammo_velocity);
    direction = sf::Vector2f{std::cos(angle), std::sin(angle)};

    this->weaponConfig = *weaponConfig;
    this->position = from;

}


