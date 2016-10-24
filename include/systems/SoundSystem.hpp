#pragma once
#include <SFML/Audio.hpp>
#include <array>
#include <events/PlaySoundEvent.hpp>

class SoundSystem final: public Observer {
public:
    void onNotify(const std::shared_ptr<GameEvent> &event) override;

    SoundSystem();

    void playMusic();

    void stopMusic();


private:
    std::array<sf::SoundBuffer, GameSound::SOUND_COUNT> soundBuffers;
    std::array<sf::Sound, GameSound::SOUND_COUNT> sounds;
    sf::Music gameMusic;

};
