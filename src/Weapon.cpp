#include "Weapon.hpp"
#include <algorithm>
#include <Debug.h>

const std::array<sf::Time, WeaponType::TYPE_COUNT>
    WeaponConfig::reloadTime = {{sf::seconds(1.0f), sf::seconds(1.0f), sf::seconds(3.0f)}};

const std::array<WeaponConfig, WEAPON_COUNT> Weapon::weaponsConfig =
    {{
         {
             SEMI_AUTOMATIC, // WeaponType type
             RPM(1000), // sf::Time cooldown
             33, // int ammo_damage
             200, // int ammo_velocity
             sf::seconds(2), // int ammo_lifetime
             false, // bool destroyable
             13 // short magazineSize
         },


         {
             AUTOMATIC,
             RPM(1000),
             25,
             300,
             sf::seconds(2),
             false,
             50
         },


         {
             PUMP_ACTION, // WeaponType type
             RPM(30), // sf::Time cooldown
             100, // int ammo_damage
             100, // int ammo_velocity
             sf::milliseconds(500), // int ammo_lifetime
             false, // bool destroyable
             4 // short magazineSize
         },

         {
             PUMP_ACTION, // WeaponType type
             RPM(5), // sf::Time cooldown
             1000,
             50,
             sf::seconds(10), // int ammo_lifetime
             true, // bool destroyable
             1 // short magazineSize
         }

     }};


Weapon::Weapon(Entity *owner, EventDispatcher &eventDispatcher, Weapons weapon)
    : owner{owner}, eventDispatcher{eventDispatcher}, currWeapon{weapon} {
    config = &weaponsConfig[weapon];
    if (config) {
        currMagazineSize = config->magazineSize;
    }

}


bool Weapon::fire() {
    static bool init_clock = false;
    if (!config) return false;
    sf::Time elapsed;
    if (!init_clock) {
        elapsed = clock.restart();
        init_clock = true;
    } else {
        elapsed = clock.getElapsedTime();
    }
    bool fire = false;
    if (elapsed >= timeToNextShot) {
        if (state == State::RELOADING) {
            state = READY;
            currMagazineSize = config->magazineSize;
        }
        fire = true;
        if (config->type == PUMP_ACTION) {
            timeToNextShot = config->reloadTime[config->type];
            state = RELOADING;
        } else {
            timeToNextShot = config->cooldown;
        }
    }

    if (fire) {
        if (currMagazineSize-- > 0) {
            clock.restart();
            return true;

        } else {
            DEBUG_MSG("Reloading");
            state = RELOADING;
            timeToNextShot = config->reloadTime[config->type];
            clock.restart();
            return false;
        }
    }
    return false;

}


const WeaponConfig *Weapon::getCurrWeaponConfig() const {
    return config;
}


const WeaponConfig *Weapon::getWeaponConfig(Weapons weapon) const {
    if (!hasWeapon(weapon)) {
        return nullptr;
    }
    return &weaponsConfig[weapon];
}


bool Weapon::setWeapon(Weapons weapon) {
    if (!hasWeapon(weapon)) {
        return false;
    }

    config = &weaponsConfig[weapon];
    currWeapon = weapon;
    return true;

}


bool Weapon::hasWeapon(Weapons weapon) const {
    //auto res = std::find(std::begin(weaponsConfig), std::end(weaponsConfig), weapon);

    return weapon <= WEAPON_COUNT;
}


void Weapon::setNoWeapon() {
    config = nullptr;

}


Weapons Weapon::getCurrWeapon() const {
    return currWeapon;
}
