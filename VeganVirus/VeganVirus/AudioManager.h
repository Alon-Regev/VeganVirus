#pragma once
#include <string>
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Winmm.lib")

class AudioManager
{
public:
	void play(std::string path);
	void changeVolume(double vol);
};

