#include "PlayState.hpp"
#include "systems/WeaponSystem.hpp"
#include <algorithm>
#include <events/FireEvent.hpp>
#include <SFML/System.hpp>
#include <GameMath.hpp>
#include "Debug.h"


void PlayState::init() {

}


PlayState::PlayState(cgf::Game *game)
    : game(game), joe{EntityConfig{}, eventDispatcher}, map{"resources/levels/"} {

    eventDispatcher.addObserver(FIRE, std::unique_ptr<Observer>(new WeaponSystem(this)));

    map.AddSearchPath("resources/sprites/");

    map.Load("level1.tmx");

    joe.setPosition(50, 100);

    DEBUG_MSG("Tamanho do map " << map.GetMapSize().x << " " << map.GetMapSize().y);
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
            case sf::Event::Resized:
                screen->setView(calcView(sf::Vector2u(event.size.width, event.size.height), idealSize));
                break;
            default:
                break;
        }

    }

    if (keyBitset.test(sf::Keyboard::LShift)) {

        eventDispatcher.notify(make_event<FireEvent>(sf::Vector2f{0, 0}, &joe, Weapon::weaponsConfig[MACHINE_GUN]));

    }

    joe.handleInput(keyBitset, buttonBitset, game);
    centerMapOnPlayer(screen);
}


void PlayState::update(cgf::Game *game) {
    auto screen = game->getScreen();
    map.UpdateQuadTree(calcViewRect(screen->getView()));

    joe.update(game);

    for (auto e: entities) {
        e->update(game);
    }
}


void PlayState::draw(cgf::Game *game) {
    auto screen = game->getScreen();
    map.Draw(*screen);
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


void PlayState::centerMapOnPlayer(sf::RenderWindow *screen) {
    sf::View view = screen->getView();
    sf::Vector2u mapsize = map.GetMapSize();
    sf::Vector2f viewsize = view.getSize();
    viewsize.x /= 2;
    viewsize.y /= 2;
    sf::Vector2f pos = joe.getPosition();

    float panX = viewsize.x; // minimum pan
    if (pos.x >= viewsize.x)
        panX = pos.x;

    if (panX >= mapsize.x - viewsize.x)
        panX = mapsize.x - viewsize.x;

    float panY = viewsize.y; // minimum pan
    if (pos.y >= viewsize.y)
        panY = pos.y;

    if (panY >= mapsize.y - viewsize.y)
        panY = mapsize.y - viewsize.y;

    sf::Vector2f center(panX, panY);
    view.setCenter(center);
    screen->setView(view);
}


sf::View PlayState::calcView(const sf::Vector2u &windowsize, const sf::Vector2u &designedsize) {
    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

    float screenwidth = windowsize.x / static_cast<float>(designedsize.x);
    float screenheight = windowsize.y / static_cast<float>(designedsize.y);

    if (screenwidth > screenheight) {
        viewport.width = screenheight / screenwidth;
        viewport.left = (1.f - viewport.width) / 2.f;
    } else if (screenwidth < screenheight) {
        viewport.height = screenwidth / screenheight;
        viewport.top = (1.f - viewport.height) / 2.f;
    }

    sf::View view(sf::FloatRect(0, 0, designedsize.x, designedsize.y));
    view.setViewport(viewport);

    return view;
}
