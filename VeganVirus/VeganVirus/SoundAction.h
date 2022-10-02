#pragma once
#include "Action.h"
#include "windows.h"
#include "AudioManager.h"


#include <chrono>
#include <ctime>  

#define MUSIC_ICON_PATH L"music.ico"
#define NUM_OF_SONGS 6
#define SOUND_DUARTION 120
#define NO_SONG -1

class SoundAction : public Action
{
public:
	SoundAction(double req);
	virtual void start();
	virtual void update(double dt);

	bool canActivate();
private:
	AudioManager _audioManager;
	double _timer;
	int currentSong = NO_SONG;
	
	const int _playTimeList[NUM_OF_SONGS] = { 160, 70, 150, 220, 270, 30 };
};

