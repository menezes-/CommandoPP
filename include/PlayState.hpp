#pragma once

#include <vector>
#include <GameState.h>
#include <tmx/MapLoader.h>
#include "Joe.hpp"


class PlayState final: public cgf::GameState {
public:
    void init() override;

    void handleEvents(cgf::Game *game) override;

    void update(cgf::Game *game) override;

    void draw(cgf::Game *game) override;

    PlayState(cgf::Game *game);

    void addEntity(Entity *entity);

    void removeEntity(Entity *entity);

private:
    void cleanup() override;

    void pause() override;

    void resume() override;

    KeyBitset keyBitset;
    MouseBitset buttonBitset;

    tmx::MapLoader map;

    std::vector<Entity *> entities;

    Joe joe;
    cgf::Game *game;
    EventDispatcher eventDispatcher;
};


