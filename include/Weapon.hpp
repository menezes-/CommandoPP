#pragma once
#include <SFML/System.hpp>
#include <array>
#include "events/EventDispatcher.hpp"

/**
 * Converte a unidade RPM (rounds per minute) em cool down time, ou seja, quanto tempo eu tenho que esperar
 * para atirar a próxima bala
 * @param rpm rounds per minute
 * @return tempo de espera entre uma bala e outra
 */
inline sf::Time RPM(float rpm) {
    return sf::seconds(60.0f / rpm);
}


enum WeaponType {
    AUTOMATIC,
    SEMI_AUTOMATIC,
    PUMP_ACTION, // revolver, espingarda etc
    TYPE_COUNT
};

enum Weapons {
    PISTOL,
    MACHINE_GUN,
    SHOTGUN,
    ZE_PEQUENO,
    WEAPON_COUNT
};

struct WeaponConfig {

    WeaponType type;

    // tempo entre um tiro e outro
    sf::Time cooldown;

    int ammo_damage;

    int ammo_velocity;

    sf::Time ammo_lifetime;

    // é possivel destruir as balas ?
    bool destroyable;

    short magazineSize;

    static const std::array<sf::Time, WeaponType::TYPE_COUNT>
        reloadTime;

};

class Entity;

class Weapon {
public:
    Weapon(Entity *owner, EventDispatcher &eventDispatcher, Weapons weapon);

    /**
     * Emite um evento de tiro
     * @return se o tiro aconteceu ou não
     */
    bool fire();

    const WeaponConfig *getCurrWeaponConfig() const;

    const WeaponConfig *getWeaponConfig(Weapons weapon) const;

    Weapons getCurrWeapon() const;

    bool hasWeapon(Weapons weapon) const;

    bool setWeapon(Weapons weapon);

    void setNoWeapon();
    static const std::array<WeaponConfig, WEAPON_COUNT> weaponsConfig;
private:
    enum State {READY, RELOADING};
    Entity *owner;
    EventDispatcher &eventDispatcher;

    int currMagazineSize; // quantas balas no pente
    sf::Time timeToNextShot;
    bool needReload{false};
    const WeaponConfig *config{nullptr};
    sf::Clock clock;
    State state{READY};
    Weapons currWeapon;


};
