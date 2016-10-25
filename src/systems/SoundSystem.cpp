#include "systems/SoundSystem.hpp"
#include "Debug.h"

void SoundSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    if (event->getEvent() != Event::PLAY_SOUND) return;

    const PlaySoundEvent *playSoundEvent = static_cast<const PlaySoundEvent *>(event.get());

    auto& sound = sounds[playSoundEvent->getGameSound()];
    if (sound.getStatus() != sf::SoundSource::Playing) {

        sound.play();

    }

}


SoundSystem::SoundSystem() {

    gameMusic.openFromFile("resources/sounds/game_music.ogg");
    gameMusic.setLoop(true);
    gameMusic.setVolume(30);

    soundBuffers[DEATH_SOUND] = sf::SoundBuffer{};
    soundBuffers[GRENADE_FLY] = sf::SoundBuffer{};

    auto &deathSound = soundBuffers[DEATH_SOUND];
    auto &grenadeFly = soundBuffers[GRENADE_FLY];

    if(!deathSound.loadFromFile("resources/sounds/death.ogg")){
        DEBUG_MSG("arquivo death.ogg não carregado");
    }
    if(!grenadeFly.loadFromFile("resources/sounds/grenade_fly.ogg")){
        DEBUG_MSG("arquivo grenade_fly não carregado");
    }


    sounds[DEATH_SOUND] = sf::Sound();
    sounds[GRENADE_FLY] = sf::Sound();
    sounds[DEATH_SOUND].setBuffer(deathSound);
    sounds[GRENADE_FLY].setBuffer(grenadeFly);

}


void SoundSystem::playMusic() {
    if (gameMusic.getStatus() != sf::SoundSource::Playing) {
        gameMusic.play();
    }

}


void SoundSystem::stopMusic() {
    if (gameMusic.getStatus() == sf::SoundSource::Playing) {
        gameMusic.stop();
    }

}

void SoundSystem::pauseMusic() {
    if (gameMusic.getStatus() == sf::SoundSource::Playing) {
        gameMusic.pause();
    }

}

void SoundSystem::toggleGameMusic(){
    if (gameMusic.getStatus() == sf::SoundSource::Playing){
        gameMusic.pause();
    } else {
        gameMusic.play();
    }
}