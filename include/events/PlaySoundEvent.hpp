#pragma once
#include "GameEvent.hpp"

enum GameSound {
    DEATH_SOUND,
    GRENADE_FLY,
    SOUND_COUNT
};

class PlaySoundEvent final: public GameEvent {
public:
    PlaySoundEvent(GameSound gameSound);

    GameSound getGameSound() const;

private:
    GameSound gameSound;

};
