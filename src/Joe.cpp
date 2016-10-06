#include "../include/Joe.hpp"
#include <InputManager.h>
#include <Debug.h>


Joe::Joe(cgf::Game *gameObj, EventDispatcher &eventDispatcher)
    : Joe(gameObj, EntityConfig{}, eventDispatcher) {

}


Joe::Joe(cgf::Game *gameObj, EntityConfig config, EventDispatcher &eventDispatcher)
    : Entity(gameObj, JOE, config, eventDispatcher) {


    load("resources/sprites/sprites_small.png", 24, 24, 0, 0, 0, 0, 15, 9);
    loadAnimation("resources/animations/joe.xml");

    directions = {{std::make_pair(0, 0), "Up"}, //default
                  {std::make_pair(0, -1), "Up"},
                  {std::make_pair(1, -1), "RightUp"},
                  {std::make_pair(1, 0), "Right"},
                  {std::make_pair(1, 1), "RightDown"},
                  {std::make_pair(0, 1), "Down"},
                  {std::make_pair(-1, 1), "LeftDown"},
                  {std::make_pair(-1, 0), "Left"},
                  {std::make_pair(-1, -1), "LeftUp"}};

    currAnimation = directions[std::make_pair(0, 0)];
    setAnimation(currAnimation);
    setAnimRate(15);

    auto im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::A);
    im->addKeyInput("right", sf::Keyboard::D);
    im->addKeyInput("up", sf::Keyboard::W);
    im->addKeyInput("down", sf::Keyboard::S);

};


void Joe::onEntityCollision(Entity &entity) {


    switch (entity.getType()) {

        case DUDE: // ao encostar em um inimigo o joe morre :(
            loseHealth(config.health);
            break;
        default:
            break;
    }

}


void Joe::handleInput(const KeyBitset &pressedKeys) {

    if (state == DYING || state == DEAD) {
        return;
    }

    auto im = cgf::InputManager::instance();

    int dirx = 0;
    int diry = 0;
    int speed = 100;

    if (im->testEvent("left")) {
        dirx = -1;
    }

    if (im->testEvent("right")) {
        dirx = 1;
    }

    if (im->testEvent("up")) {
        diry = -1;
    }

    if (im->testEvent("down")) {
        diry = 1;
    }

#ifndef NDEBUG
    if (pressedKeys.test(sf::Keyboard::L)) {
        loseHealth(config.health / 2);
    }
#endif

    // se andando na diagonal a velocidade é menor
    if (dirx != 0 && diry != 0) {
        speed *= 0.85;
    }

    if (dirx == 0 && diry == 0) {
        pause();
        setAndPlay("");
    } else {
        auto pair = std::make_pair(dirx, diry);
        auto anim_find = directions.find(pair);
        if (anim_find == directions.end()) {
            DEBUG_MSG("direção " << dirx << ", " << diry << " não encontrada!");
            // vai pra animação default
            anim_find = directions.find(std::make_pair(0, 0));
        }
        setAndPlay(anim_find->second);
    }

    setXspeed(speed * dirx);
    setYspeed(speed * diry);

}


void Joe::update() {

    if (state == DYING && currAnimation != "Die") {
        setAndPlay("Die");
        setAnimRate(2);

    }

    Entity::update();
}
