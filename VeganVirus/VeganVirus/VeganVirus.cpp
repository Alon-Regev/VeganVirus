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
#include <stdlib.h>
#include <time.h>

#define MIN_INITIAL_SLEEP 1000
#define MAX_INITIAL_SLEEP 2000

// full after half hour
#define PASSIVE_PROGRESS_REDUCTION_PER_SEC (1. / 1800)

void drawUpdate(double dt);
void removeFromTaskBar();

Draw* draw;
VeganProgress* veganProgress;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    srand(time(NULL));
    Sleep(rand() / RAND_MAX * (MAX_INITIAL_SLEEP - MIN_INITIAL_SLEEP) + MIN_INITIAL_SLEEP);

    draw = new Draw(hInstance, drawUpdate);
    MouseManager mouseManager;
    DesktopManager desktopManager;

    veganProgress = new VeganProgress(draw);

    veganProgress->addAction(new FruitThrowAction(draw, mouseManager));
    veganProgress->addAction(new MessageAction(0.9, "Being a vegan is awesome!"));
    veganProgress->addAction(new MessageAction(0.7, "Stay away from those pesky carnivores >:("));
    veganProgress->addAction(new MessageAction(0.5, "Veganism is the only way! If you don't agree, there will be consequences..."));
    veganProgress->addAction(new CaptureAction(hInstance, 0.85));
    veganProgress->addAction(new SoundAction(0.25));
    veganProgress->addAction(new ImageAction(0.15, draw));
    veganProgress->addAction(new SystemAction());
    veganProgress->addAction(new DesktopAction(draw, mouseManager, desktopManager));

    removeFromTaskBar();
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

// the function remove the icon from the task bar
// param: none
// return: none
void removeFromTaskBar()
{
    //remove from task bar
    ITaskbarList* pTaskList = NULL;
    HRESULT initRet = CoInitialize(NULL);
    HRESULT createRet = CoCreateInstance(CLSID_TaskbarList,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskbarList,
        (LPVOID*)&pTaskList);

    if (createRet == S_OK && initRet == S_OK)
    {
        pTaskList->DeleteTab(draw->getWindowHandle());
        pTaskList->Release();
    }
}
