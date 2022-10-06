#include "AudioManager.h"

void AudioManager::play(std::string path)
{
    mciSendStringA(path.c_str(), NULL, 0, 0);
}

void AudioManager::changeVolume(double nVolume)
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
