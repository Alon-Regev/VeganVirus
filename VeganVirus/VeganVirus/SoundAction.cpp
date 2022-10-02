#include "SoundAction.h"

SoundAction::SoundAction(double req) : Action(req, MUSIC_ICON_PATH)
{
    playListTime[0] = 62;
    start();
}

void SoundAction::start()
{
    if (!canActivate())
    {
        return;
    }
    std::string song = "";
    //change the volume to 30 for now
    this->_audioManager.changeVolume(30);

    //choose randomly song
    this->currentSong = rand() % NUM_OF_SONGS;
    song = "play music\\song" + std::to_string(this->currentSong) + ".wav";

    //play and check when song start
    this->_audioManager.play(song);
    this->songStartTime = clock();
}


bool SoundAction::canActivate()
{
    //check that song is playing
    if (this->currentSong != -1)
    {
        //substract the current time by the start time
        clock_t duration = clock() - this->songStartTime;
        //convert ms to s
        duration /= 1000;
        if (duration < playListTime[currentSong])
        {
            return false;
        }
    }
    return true;
}


