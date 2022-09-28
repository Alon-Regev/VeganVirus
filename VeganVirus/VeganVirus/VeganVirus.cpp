#include "Draw.h"

Draw* draw;

void drawUpdate(double dt);
void removeFromTaskBar();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    draw = new Draw(hInstance, drawUpdate);

    removeFromTaskBar();
    while (draw->update());

    delete draw;
    return 0;

}

// the function updates the screen
void drawUpdate(double dt)
{

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
        pTaskList->DeleteTab(draw->_hwnd);
        pTaskList->Release();
    }
}
