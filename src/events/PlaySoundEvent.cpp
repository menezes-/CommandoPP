#include "events/PlaySoundEvent.hpp"


PlaySoundEvent::PlaySoundEvent(GameSound gameSound)
    : GameEvent(Event::PLAY_SOUND), gameSound{gameSound} {

}


GameSound PlaySoundEvent::getGameSound() const {
    return gameSound;
}


