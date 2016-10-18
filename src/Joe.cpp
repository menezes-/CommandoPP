#include "Joe.hpp"
#include <InputManager.h>
#include <Debug.h>
#include <events/FireEvent.hpp>
#include "GameMath.hpp"


Joe::Joe(EventDispatcher &eventDispatcher)
    : Joe(EntityConfig{}, eventDispatcher) {

}


Joe::Joe(EntityConfig config, EventDispatcher &eventDispatcher)
    : Entity(JOE, config, eventDispatcher), weapon{this, eventDispatcher, MACHINE_GUN} {

    loadSmallSprites();
    loadAnimation("resources/animations/joe.xml");

    directions = {{Dir(0, 0), "Up"}, //default
                  {Dir(0, -1), "Up"},
                  {Dir(1, -1), "RightUp"},
                  {Dir(1, 0), "Right"},
                  {Dir(1, 1), "RightDown"},
                  {Dir(0, 1), "Down"},
                  {Dir(-1, 1), "LeftDown"},
                  {Dir(-1, 0), "Left"},
                  {Dir(-1, -1), "LeftUp"}};

    currAnimation = directions[Dir(0, 0)];
    setAnimation(currAnimation);
    setAnimRate(15);

    auto im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::A);
    im->addKeyInput("right", sf::Keyboard::D);
    im->addKeyInput("up", sf::Keyboard::W);
    im->addKeyInput("down", sf::Keyboard::S);
    im->addMouseInput("fire", sf::Mouse::Left);

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


void Joe::onMapCollision(
    tmx::MapObject *mapObject,
    const sf::FloatRect &overlap,
    const sf::Vector2f &collisionNormal

) {
    //http://trederia.blogspot.com.br/2016/02/2d-physics-101-pong.html
    auto manifold = getManifold(overlap, collisionNormal);

    auto offset = sf::Vector2f(manifold.x, manifold.y) * manifold.z;
    move(offset);

}


void Joe::handleInput(const KeyBitset &pressedKeys, const MouseBitset &pressedButtons, cgf::Game *gameObj) {
    if (state != ALIVE) return;
    static auto im = cgf::InputManager::instance();

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
        auto pair = Dir(dirx, diry);
        auto anim_find = directions.find(pair);
        if (anim_find == directions.end()) {
            DEBUG_MSG("direção " << dirx << ", " << diry << " não encontrada!");
            // vai pra animação default
            anim_find = directions.find(Dir(0, 0));
        }
        setAndPlay(anim_find->second);
        play();
    }

    auto screen = gameObj->getScreen();
    auto view = screen->getView();
    auto viewRect = calcViewRect(view);

    if (weapon.getCurrWeaponConfig()) {
        if (weapon.getCurrWeaponConfig()->type == AUTOMATIC) {
            if (im->testEvent("fire") && weapon.fire()) {
                auto mouse = screen->mapPixelToCoords(sf::Mouse::getPosition(*screen));
                if (viewRect.contains(mouse)) {
                    auto dir = getMouseDirectionFromPosition(getPosition(), gameObj->getScreen());
                    eventDispatcher.notify(make_event<FireEvent>(this, dir));
                }
            }
        } else {
            if (pressedButtons.test(sf::Mouse::Left) && weapon.fire()) {
                auto mouse = screen->mapPixelToCoords(sf::Mouse::getPosition(*screen));
                if (viewRect.contains(mouse)) {
                    auto dir = getMouseDirectionFromPosition(getPosition(), gameObj->getScreen());
                    eventDispatcher.notify(make_event<FireEvent>(this, dir));
                }
            }
        }

    }

    setXspeed(speed * dirx);
    setYspeed(speed * diry);

}


Weapon &Joe::getWeapon() {
    return weapon;
}


void Joe::die() {
    setAndPlay("Die");
    setAnimRate(5);

    Entity::die();
}
