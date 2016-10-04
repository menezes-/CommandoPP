#include "../include/PlayState.hpp"


void PlayState::init() {

}


void PlayState::handleEvents(cgf::Game *game) {
    auto screen = game->getScreen();
    sf::Event event;

    while (screen->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game->quit();
            return;
        }
    }
    joe.handleInput();
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
