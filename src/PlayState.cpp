#include "PlayState.hpp"
#include "WeaponSystem.hpp"
#include <algorithm>
#include <events/FireEvent.hpp>
#include <SFML/System.hpp>

void PlayState::init() {

}


PlayState::PlayState(cgf::Game *game)
    : game(game), joe{EntityConfig{}, eventDispatcher} {
    eventDispatcher.addObserver(FIRE, std::unique_ptr<Observer>(new WeaponSystem(this)));

}


void PlayState::handleEvents(cgf::Game *game) {
    auto screen = game->getScreen();
    sf::Event event;
    keyBitset.reset();
    buttonBitset.reset();
    while (screen->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                game->quit();
                return;
            case sf::Event::KeyPressed:
                keyBitset.set(event.key.code);
                break;
            case sf::Event::MouseButtonPressed:
                buttonBitset.set(event.mouseButton.button);
                break;
            default:
                break;
        }

    }
#ifndef NDEBUG
    if (keyBitset.test(sf::Keyboard::LShift)) {

        eventDispatcher.notify(make_event<FireEvent>(sf::Vector2f{0,0}, &joe, Weapon::weaponsConfig[MACHINE_GUN]));

    }
#endif
    joe.handleInput(keyBitset, buttonBitset, game);
}


void PlayState::update(cgf::Game *game) {
    joe.update(game);

    for (auto e: entities) {
        e->update(game);
    }
}


void PlayState::draw(cgf::Game *game) {
    auto screen = game->getScreen();
    screen->draw(joe);
    for (auto e: entities) {
        if (e->getState() == DEAD) continue;
        screen->draw(*e);
    }
}


void PlayState::addEntity(Entity *entity) {
    entities.push_back(entity);
}


void PlayState::removeEntity(Entity *entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}


void PlayState::cleanup() {

}


void PlayState::pause() {

}


void PlayState::resume() {

}
