#pragma once

#include <vector>
#include <GameState.h>
#include <tmx/MapLoader.h>
#include "HUD.hpp"
#include "Joe.hpp"
#include "EntityManager.hpp"
#include "systems/CollisionSystem.hpp"
#include "tmx/MapObject.h"
#include <unordered_map>

class PlayState final: public cgf::GameState {
public:
    void init() override;

    void handleEvents(cgf::Game *game) override;

    void update(cgf::Game *game) override;

    void draw(cgf::Game *game) override;

    PlayState(cgf::Game *game);

private:
    void cleanup() override;

    void pause() override;

    void resume() override;

    KeyBitset keyBitset;
    MouseBitset mouseBitset;

    tmx::MapLoader map;

    Joe* joe;
    cgf::Game *game;
    EventDispatcher eventDispatcher;

    CollisionSystem collisionSystem;

    EntityManager entityManager;

    void centerMapOnPlayer(sf::RenderWindow *);

    const sf::Vector2u idealSize{336, 600};

    sf::View calcView(const sf::Vector2u &windowsize, const sf::Vector2u &designedsize);

    void computeEntityMapCollision();

    sf::RectangleShape getOutline(Entity &entity);

    void computeEntityCollision();

    sf::View HUDView;
    HUD hud;

    bool isPaused{false};

    // indica se o jogo foi pausado pelo usuário
    // se foi pausado pelo usuário ao receber o evento de GainedFocus
    // não "despausa" o jogo.
    bool pausedByUser{true};

    /*
     * inicialmente objects seria um vetor "indexado" pelo id dos objetos
     * porém de acordo com a documentação do formato tmx, se um objeto é deletado do mapa
     * o id que ele continha nunca mais é utilizado, o que cria a possibilidade de buracos na
     * sequência de IDs. Além disso a descrição do formato não indica em lugar nenhum que o id tem que ser
     * numérco
     * http://doc.mapeditor.org/reference/tmx-map-format/#object
     */
    std::unordered_map<std::string, tmx::MapObject*> objects;
    std::unordered_map<std::string, bool> objectsInView;
};
