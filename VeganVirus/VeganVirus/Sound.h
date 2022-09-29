#pragma once
#include "Action.h"
#include "windows.h"
#include "mmsystem.h"
#include <Mmsystem.h>
#include <mciapi.h>
#include <mmeapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

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
	clock_t songStartTime;
	int currentSong = -1;
	//how many second it takes until the song end
	int playListTime[NUM_OF_SONGS];
};

