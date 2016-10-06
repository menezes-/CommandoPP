#include "../include/PlayState.hpp"


void PlayState::init() {

}


void PlayState::handleEvents(cgf::Game *game) {
    auto screen = game->getScreen();
    sf::Event event;
    keyBitset.reset();
    while (screen->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                game->quit();
                return;
            case sf::Event::KeyPressed:
                keyBitset.set(event.key.code);
                break;
            default:
                break;
        }

    }
    joe.handleInput(keyBitset);
}


void PlayState::update(cgf::Game *game) {
    joe.update();
}


void PlayState::draw(cgf::Game *game) {
    auto screen = game->getScreen();
    screen->draw(joe);
}


PlayState::PlayState(cgf::Game *game)
    : game(game), joe{game, EntityConfig{}, eventDispatcher} {

}


void PlayState::cleanup() {

}


void PlayState::pause() {

}


void PlayState::resume() {

}
