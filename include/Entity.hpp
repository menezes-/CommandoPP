#pragma once

#include <Sprite.h>
#include <Game.h>


enum EntityType: short {
    JOE,
    DUDE
};

enum EntityState: short {
    ALIVE,
    DEAD,
    DYING
};

struct EntityConfig {
    int lives{5};
    int health{100};
    bool godMode{false};
    bool movable{true};

};

class Entity: public cgf::Sprite {

public:
    virtual void onEntityCollision(Entity &entity) =0;
    virtual void handleInput() {};
    virtual void update();
    const EntityConfig &getConfig() const;
    void setConfig(EntityConfig config);
    void loseHealth(int amount);

protected:
    Entity(cgf::Game *gameObj, EntityType type, EntityConfig config);
    cgf::Game *gameObj;
    EntityType type;
    EntityState state{ALIVE};
    EntityConfig config;
private:
    int lives;
    int health;

};


