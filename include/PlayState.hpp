#pragma once

#include <GameState.h>
#include "Joe.hpp"
#include "EventDispatcher.hpp"


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

private:
    Joe joe;
    cgf::Game *game;
    EventDispatcher eventDispatcher;
};


