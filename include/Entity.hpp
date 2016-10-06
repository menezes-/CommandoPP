#pragma once

#include <Sprite.h>
#include <Game.h>
#include <bitset>
#include <SFML/Window/Keyboard.hpp>
#include "EventDispatcher.hpp"

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

//Bitset que guarda todas as teclas pressionadas
using KeyBitset = std::bitset<sf::Keyboard::KeyCount>;

class EventDispatcher;

class Entity: public cgf::Sprite {

public:
    virtual void onEntityCollision(Entity &entity) =0;

    virtual void handleInput(const KeyBitset &pressedKeys) {};

    virtual void update();

    const EntityConfig &getConfig() const;

    void setConfig(EntityConfig config);

    void loseHealth(int amount);

    EntityType getType() const;

    EntityState getState() const;

    int getLives() const;

    int getHealth() const;

    virtual ~Entity() {}


protected:
    Entity(cgf::Game *gameObj,
           EntityType type,
           EntityConfig config,
           EventDispatcher &eventDispatcher);

    cgf::Game *gameObj;
    EntityType type;
    EntityState state{ALIVE};
    EntityConfig config;
    int lives;
    int health;
    EventDispatcher &eventDispatcher;
    std::string currAnimation;
    void setAndPlay(std::string name);

};


