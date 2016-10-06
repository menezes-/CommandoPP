#include <Game.h>
#include "include/PlayState.hpp"


int main() {
    cgf::Game game{30, 60};
    PlayState playState{&game};
    game.init("Commando++", 800, 600, false);
    game.changeState(&playState);

    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.draw();
    }

    // cleanup the engine
    game.clean();

    return 0;

}