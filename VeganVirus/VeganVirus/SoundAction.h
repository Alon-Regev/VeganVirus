#pragma once
#include "Action.h"
#include "windows.h"
#include "AudioManager.h"


#include <chrono>
#include <ctime>  

#define MUSIC_ICON_PATH L"music.ico"
#define NUM_OF_SONGS 6
#define SOUND_DUARTION 120


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
	int currentSong = -1;
	
	int playTimeList[NUM_OF_SONGS] = { 160, 70, 150, 220, 270, 30 };
};

