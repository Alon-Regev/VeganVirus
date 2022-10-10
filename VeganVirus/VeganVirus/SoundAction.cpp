#include "SoundAction.h"

SoundAction::SoundAction(double req, AudioManager& audioManager) 
    : Action(req, MUSIC_ICON_PATH), _audioManager(audioManager)
{
    _timer = SOUND_DUARTION + 1;
}

void SoundAction::start()
{
    this->_audioManager.changeVolume(60);
    _timer = 0;

    //choose randomly song
    this->_currentSong = rand() % NUM_OF_SONGS;
    std::string song = "music\\song" + std::to_string(this->_currentSong) + ".wav";

    //play and check when song start
    this->_audioManager.play(song);
}

void SoundAction::update(double dt)
{
    _timer += dt;
}


bool SoundAction::canActivate()
{    
    return this->_currentSong == NO_SONG || _timer > this->_playTimeList[this->_currentSong];
}


