#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"

class Dude final: public Entity {
public:
    Dude(EntityConfig config, EventDispatcher &eventDispatcher);

    Dude(EventDispatcher &eventDispatcher);

    void setEnemy(Entity *enemy);


    void onEntityCollision(Entity &entity) override;

    void onMapCollision(
        tmx::MapObject *mapObject,
        const sf::FloatRect &overlap,
        const sf::Vector2f &collisionNormal

    ) override;

    void setPanic(bool panic);

    void setWeapon(Weapons weapon);

    void setError(float error);

    void update(cgf::Game *gameObj) override;

    void die() override;


private:
    std::map<Dir, std::string> directionAnimation;

    Entity* enemy{nullptr};

    Dir currDir{0, 1};
    bool panic{false};
    bool panicMode{false};
    Weapon weapon;
    float error{0.f};

};
