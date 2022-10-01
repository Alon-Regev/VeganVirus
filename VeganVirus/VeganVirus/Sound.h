#pragma once
#include "Action.h"
#include "windows.h"
#include "AudioManager.h"


#include <chrono>
#include <ctime>  

#define MUSIC_ICON_PATH L"music.ico"
#define NUM_OF_SONGS 1



class Sound : public Action
{
public:
	Sound(double req);
	void start();

	void changeVolume(double vol);
	bool canActivate();
private:
	AudioManager _audioManager;
	clock_t songStartTime;
	int currentSong = -1;
	//how many second it takes until the song end
	int playListTime[NUM_OF_SONGS];
};

