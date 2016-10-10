#pragma once

#include <Sprite.h>
#include <Game.h>
#include <bitset>
#include <utility>
#include <SFML/Window/Keyboard.hpp>
#include "events/EventDispatcher.hpp"

enum EntityType: short {
    JOE,
    DUDE,
    BULLET,

};

enum EntityState: short {
    ALIVE,
    DEAD,
    DYING
};

struct EntityConfig {
    EntityConfig(int lives, int health, bool godMode, bool movable);

    EntityConfig() {}

    int lives{5};
    int health{100};
    bool godMode{false};
    bool movable{true};

};

//Bitset que guarda todas as teclas pressionadas
using KeyBitset = std::bitset<sf::Keyboard::KeyCount>;
using MouseBitset = std::bitset<sf::Mouse::ButtonCount>;

// direction
using Dir = std::pair<int, int>;

class EventDispatcher;

class Entity: public cgf::Sprite {

public:
    virtual void onEntityCollision(Entity &other) =0;

    virtual void handleInput(const KeyBitset &pressedKeys, const MouseBitset &pressedButtons) {};

    virtual void update(cgf::Game *gameObj);

    virtual operator std::string() const;

    const EntityConfig &getConfig() const;

    void setConfig(EntityConfig config);

    void loseHealth(int amount);

    EntityType getType() const;

    EntityState getState() const;

    int getLives() const;

    int getHealth() const;

    int getId();

    virtual ~Entity() {}


protected:
    Entity(EntityType type,
           EntityConfig config,
           EventDispatcher &eventDispatcher);

    EntityType type;
    EntityState state{ALIVE};
    EntityConfig config;
    int lives;
    int health;
    EventDispatcher &eventDispatcher;
    std::string currAnimation;
    int id;
    //id da entidade
    static int s_id;

    void setAndPlay(std::string name);

    bool isEnemy(const Entity &other);

    /**
     * Função de utilidade que faz um load do arquivo sprites_small.png
     */
    void loadSmallSprites();

};


