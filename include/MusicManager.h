#ifndef CS437_MUSIC_MANAGER_H
#define CS437_MUSIC_MANAGER_H

#include <SFML/Audio.hpp>
#include <string>
#include "engine.h"

class MusicManager {
private:
    std::shared_ptr<sf::Music> bg_music;
    bool initialized = false;
    std::string curTrack;

protected:


public:
    void setMusic(std::string);

    void setVolume(float);

    void playMusic();

    void stopMusic();

    std::string getCurrentTrack();
};

extern MusicManager music;

#endif //CS427_MUSIC_MANAGER_H
