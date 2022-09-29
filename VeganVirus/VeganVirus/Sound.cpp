#include "Sound.h"

Sound::Sound(double req) : Action(req, MUSIC_ICON_PATH)
{
    playListTime[0] = 62;
}

void Sound::start()
{
    if (!canActivate())
    {
        return;
    }
    std::string command = "";
    //change the volume to 30 for now
    changeVolume(30);

    //choose randomly song
    this->currentSong = rand() % NUM_OF_SONGS;
    command = "play music\\song" + std::to_string(this->currentSong) + ".wav";

    //play and check when song start
    //mciSendStringA(command.c_str(), NULL, 0, 0);
    this->songStartTime = clock();
}


bool Sound::canActivate()
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


void Sound::changeVolume(double nVolume)
{
    HRESULT hr = NULL;
    bool decibels = false;
    bool scalar = false;
    double newVolume = nVolume / 100;

    CoInitialize(NULL);
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    IMMDevice* defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume* endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
        CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    // -------------------------
    float currentVolume = 0;
    endpointVolume->GetMasterVolumeLevel(&currentVolume);
    //printf("Current volume in dB is: %f\n", currentVolume);

    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
    
    endpointVolume->Release();

    CoUninitialize();
}