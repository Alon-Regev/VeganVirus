#include "SoundAction.h"

SoundAction::SoundAction(double req) : Action(req, MUSIC_ICON_PATH)
{
    _timer = SOUND_DUARTION + 1;
}

void SoundAction::start()
{
    this->_audioManager.changeVolume(60);
    _timer = 0;
}

void SoundAction::update(double dt)
{
    std::string song = "";

    _timer += dt;
    if (!canActivate() || _timer >= SOUND_DUARTION)
    {
        return;
    }
    

    //choose randomly song
    this->currentSong = rand() % NUM_OF_SONGS;
    song = "play music\\song" + std::to_string(this->currentSong) + ".wav";

    //play and check when song start
    this->_audioManager.play(song);
}


bool SoundAction::canActivate()
{    
    //check that song no is playing
    if (this->currentSong != NO_SONG && _timer <= _playTimeList[currentSong])
    {
        return false;  
    }
    return true;
}


