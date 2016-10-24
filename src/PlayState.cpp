#include "PlayState.hpp"
#include <events/FireEvent.hpp>
#include <events/CollisionEvent.hpp>
#include <GameMath.hpp>
#include <cstdlib>
#include <events/MObjectInView.hpp>


void PlayState::init() {

}


PlayState::PlayState(cgf::Game *game)
    : game(game), map{"resources/levels/"}, entityManager{&eventDispatcher},
      collisionSystem{}, hud(entityManager, isPaused) {

    joe = entityManager.getJoe();
    entityManager.setEventDispatcher(&eventDispatcher);
    eventDispatcher.addObserver(&collisionSystem, Event::COLLISION_EVENT);
    eventDispatcher.addObserver(&entityManager, Event::FIRE, Event::ENTITY_IS_DEAD);
    entityManager.generateBullets(entityManager.bulletCacheSize);
    map.AddSearchPath("resources/sprites/");

    map.Load("level1.tmx");

    bool joeIniPos{false};
    for (auto &layer : map.GetLayers()) {
        if (layer.name == "objects") {
            auto objSize = layer.objects.size();
            objectsInView.reserve(objSize);
            for (auto &obj: layer.objects) {
                if (obj.GetName() == "hero" && !joeIniPos) {
                    joe->setPosition(obj.GetPosition().x, obj.GetAABB().top);
                    // só por precaução
                    joeIniPos = true;
                }

                objectsInView.insert(std::make_pair(&obj, false));

            }
            break;
        }
    }

}


void PlayState::handleEvents(cgf::Game *game) {
    auto screen = game->getScreen();
    sf::Event event;
    keyBitset.reset();
    mouseBitset.reset();
    while (screen->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                game->quit();
                return;
            case sf::Event::KeyPressed:
                if (event.key.code != sf::Keyboard::Unknown) {
                    keyBitset.set(event.key.code);
                }
                break;
            case sf::Event::MouseButtonPressed:
                mouseBitset.set(event.mouseButton.button);
                break;
            case sf::Event::Resized:
                screen->setView(calcView(sf::Vector2u(event.size.width, event.size.height), idealSize));
                if (isPaused) {
                    centerMapOnPlayer(screen);
                }
                break;
            case sf::Event::LostFocus:
                if (!isPaused) {
                    pausedByUser = false;
                    pause();
                }
                break;
            case sf::Event::GainedFocus:
                if (!pausedByUser) {
                    pausedByUser = true;
                    resume();
                }
                break;
            default:
                break;
        }

    }

    if (keyBitset.test(sf::Keyboard::P)) {
        if (!isPaused) {
            pausedByUser = true;
            pause();
        } else {
            resume();
        }
    }

    if (isPaused) {
        return;
    }

    if (keyBitset.test(sf::Keyboard::LShift)) {
        eventDispatcher.notify(make_event<FireEvent>(joe->getPosition() + sf::Vector2f(0, -200),
                                                     joe,
                                                     Weapon::weaponsConfig[MACHINE_GUN]));

    }

    joe->handleInput(keyBitset, mouseBitset, game);
    centerMapOnPlayer(screen);
}


void PlayState::update(cgf::Game *game) {
    if (isPaused) {
        return;
    }
    auto screen = game->getScreen();
    auto viewRect = calcViewRect(screen->getView());
    map.UpdateQuadTree(viewRect);
    eventDispatcher.dispatchEvents();
    entityManager.update(viewRect);
    computeEntityMapCollision();
    computeObjectsInView(viewRect);
    computeEntityCollision();
    hud.update();

    for (auto e: entityManager) {
        e->update(game);
    }
}


void PlayState::draw(cgf::Game *game) {
    auto screen = game->getScreen();
    auto currView = screen->getView();
    //map.Draw(*screen, tmx::MapLayer::DrawType::Debug, true);
    map.Draw(*screen);
    for (auto e: entityManager) {
        screen->draw(*e);
    }

    screen->setView(HUDView);
    hud.draw(screen);
    screen->setView(currView);

}


void PlayState::cleanup() {

}


void PlayState::pause() {
    if (!isPaused) {
        isPaused = true;
        hud.resetPausedClock();
    }
}


void PlayState::resume() {
    isPaused = false;
}


void PlayState::centerMapOnPlayer(sf::RenderWindow *screen) {
    sf::View view = screen->getView();
    sf::Vector2u mapsize = map.GetMapSize();
    sf::Vector2f viewsize = view.getSize();
    viewsize.x /= 2;
    viewsize.y /= 2;
    sf::Vector2f pos = joe->getPosition();

    float panX = viewsize.x; // minimum pan
    if (pos.x >= viewsize.x) {
        panX = pos.x;
    }

    if (panX >= mapsize.x - viewsize.x) {
        panX = mapsize.x - viewsize.x;
    }

    float panY = viewsize.y; // minimum pan
    if (pos.y >= viewsize.y) {
        panY = pos.y;
    }

    if (panY >= mapsize.y - viewsize.y) {
        panY = mapsize.y - viewsize.y;
    }

    sf::Vector2f center(panX, panY);
    view.setCenter(center);
    screen->setView(view);
}


sf::View PlayState::calcView(const sf::Vector2u &windowsize, const sf::Vector2u &designedsize) {
    sf::FloatRect viewport(0.f, 0.f, 1.f, .96f);

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
    HUDView = view;
    HUDView.setViewport(viewport);
    viewport.top += 0.04f;
    view.setViewport(viewport);
    return view;
}


void PlayState::computeEntityCollision() {
    auto size = entityManager.size();
    std::vector<bool> passados(size * size, false);

    for (auto x: entityManager) {
        for (auto y: entityManager) {
            if (x == y) continue;
            if (x->getState() != ALIVE || y->getState() != ALIVE) continue;
            auto pIndex = x->getId() * y->getId() + size;
            if (passados[pIndex]) continue;

            if (x->isEnemy(*y) && x->bboxCollision(*y)) {
                eventDispatcher.notify(make_event<CollisionEvent>(x, y));
                passados[pIndex] = true;
            }
        }
    }

}


void PlayState::computeEntityMapCollision() {


    for (auto entity: entityManager) {
        for (auto object: map.QueryQuadTree(entity->getGlobalBounds())) {
            // meu objeto é "collidable"?
            if (object->GetParent() != "collision") continue;

            auto bbox = entity->getBoundingBox();
            sf::FloatRect rect{sf::Vector2f{entity->getPosition().x,
                                            entity->getPosition().y},
                               sf::Vector2f{bbox.width, bbox.height}};
            sf::FloatRect overlap;
            if (object->GetAABB().intersects(rect, overlap)) {
                auto normal = object->GetCentre() - entity->getPosition();
                eventDispatcher.notify(make_event<CollisionEvent>(entity, object, overlap, normal));

            }

        }
    }

}


void PlayState::computeObjectsInView(const sf::FloatRect &viewRect) {
    for(auto& obj: objectsInView){
        auto mapObject = obj.first;
        if(viewRect.contains(mapObject->GetCentre())){
            if(!obj.second){
                eventDispatcher.notify(make_event<MObjectInView>(mapObject));
                obj.second = true;
            }

        } else {
            if(obj.second){
                obj.second = false;
            }
        }
    }
}


sf::RectangleShape PlayState::getOutline(Entity &entity) {
    auto rect = entity.getBoundingBox();
    auto rectangle = sf::RectangleShape{sf::Vector2f{rect.width, rect.height}};
    rectangle.setPosition(entity.getPosition().x + rect.width / 2, entity.getPosition().y + rect.height / 2);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Yellow);
    rectangle.setOutlineThickness(1);
    return rectangle;
}
