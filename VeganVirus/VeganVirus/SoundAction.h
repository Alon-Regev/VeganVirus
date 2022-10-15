#pragma once
#include "Action.h"
#include "windows.h"
#include "AudioManager.h"


#include <chrono>
#include <ctime>  

#define MUSIC_ICON_PATH L"Images\\music.ico"
#define NUM_OF_SONGS 5
#define SOUND_DUARTION 120
#define NO_SONG -1

class SoundAction : public Action
{
public:
	SoundAction(double req, AudioManager& audioManager);
	virtual void start();
	virtual void update(double dt);

	bool canActivate();
private:
	AudioManager& _audioManager;
	double _timer;
	int _currentSong = NO_SONG;
	
	const int _playTimeList[NUM_OF_SONGS] = { 30, 30, 30, 30, 30};
};

