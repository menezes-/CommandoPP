#pragma once
#include "GameEvent.hpp"
#include "Weapon.hpp"
#include <SFML/System.hpp>
class FireEvent final: public GameEvent {
public:


    /**
     * Atira na direção especificada por dirx e diry
     * @param from quem atirou
     * @param direction direção do tiro
     */
    FireEvent(Entity *from, sf::Vector2f direction);

    const WeaponConfig &getWeaponConfig() const;

    sf::Vector2f getDirection() const;

    sf::Vector2f getPosition() const;

    /**
     * Essa função serve apenas para teste.
     * Permite um tiro vir de uma posição qualquer
     * (idealmente um tiro sempre deve vir de uma entidade)
     */
     FireEvent(sf::Vector2f from, Entity *target, const WeaponConfig* weaponConfig, float error);

private:
    WeaponConfig weaponConfig;
    sf::Vector2f direction;
    sf::Vector2f position;

};