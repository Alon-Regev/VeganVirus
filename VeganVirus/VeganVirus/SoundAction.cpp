#include "SoundAction.h"

SoundAction::SoundAction(double req) : Action(req, MUSIC_ICON_PATH)
{
    _timer = 0;
}

void SoundAction::start()
{
    _timer = SOUND_DUARTION;
}

void SoundAction::update(double dt)
{
    std::string song = "";
    _timer -= dt;
    if (_timer <= 0 || !canActivate())
    {
        return;
    }
    
    //change the volume to 30 for now
    this->_audioManager.changeVolume(60);

    //choose randomly song
    this->currentSong = rand() % NUM_OF_SONGS;
    song = "play music\\song" + std::to_string(this->currentSong) + ".wav";

    //play and check when song start
    this->_audioManager.play(song);
}


bool SoundAction::canActivate()
{
    double timePass = SOUND_DUARTION - _timer;
    
    //check that song no is playing
    if (this->currentSong != -1)
    {
        if (timePass <= playTimeList[currentSong])
        {
            return false;
        }  
    }
    return true;
}


