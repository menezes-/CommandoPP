#include "systems/SoundSystem.hpp"


void SoundSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    if (event->getEvent() != Event::PLAY_SOUND) return;

    const PlaySoundEvent *playSoundEvent = static_cast<const PlaySoundEvent *>(event.get());

    auto sound = sounds[playSoundEvent->getGameSound()];
    if (sound.getStatus() == sf::Sound::Stopped) {
        auto wasPlaying = gameMusic.getStatus() == sf::Sound::Playing;
        if (wasPlaying) {
            gameMusic.stop();
        }

        sound.play();

        if (wasPlaying) {
            gameMusic.play();
        }

    }

}


SoundSystem::SoundSystem() {

    gameMusic.openFromFile("resources/sounds/game_music.ogg");
    gameMusic.setLoop(true);
    sf::SoundBuffer deathSound;
    sf::SoundBuffer grenadeFly;
    deathSound.loadFromFile("resources/sounds/death.ogg");
    grenadeFly.loadFromFile("resources/sounds/grenade_fly.ogg");
    soundBuffers[DEATH_SOUND] = deathSound;
    soundBuffers[GRENADE_FLY] = grenadeFly;

    sounds[DEATH_SOUND] = sf::Sound{soundBuffers[DEATH_SOUND]};
    sounds[GRENADE_FLY] = sf::Sound{soundBuffers[GRENADE_FLY]};

    gameMusic.setVolume(30);

}


void SoundSystem::playMusic() {
    if (gameMusic.getStatus() == sf::Sound::Stopped) {
        gameMusic.play();
    }

}


void SoundSystem::stopMusic() {
    if (gameMusic.getStatus() == sf::Sound::Playing) {
        gameMusic.stop();
    }

}

void SoundSystem::toggleGameMusic(){
    if (gameMusic.getStatus() == sf::Sound::Playing){
        gameMusic.stop();
    } else {
        gameMusic.play();
    }
}