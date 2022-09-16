#include "draw_to_screen.h"
#include <math.h>

#define IMAGE_PATH L"C:\\cat.png"

Draw* draw;
Bitmap* bmp;

void drawUpdate(double dt);
void removeFromTaskBar();
void duplicateWindow(int numberOfWindows, int x, int y);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{   
    draw = new Draw(hInstance, drawUpdate);
    bmp = new Bitmap(IMAGE_PATH);

    //removeFromTaskBar();
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
    //draw->drawImage(bmp, x, y);

    duplicateWindow(draw->numOfWindows, 10, 10);
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

void duplicateWindow(int numberOfWindows, int x, int y)
{

    RectF sourceRect(
        0.0f,
        0.0f,
        (REAL)bmp->GetWidth(),
        (REAL)bmp->GetHeight());
    

    for (int i = 0; i < numberOfWindows; i++)
    {
        x = x > GetSystemMetrics(SM_CXSCREEN) ? 0 : x + 60;
        y = y > GetSystemMetrics(SM_CYSCREEN) ? 0 : y + 60;
        Bitmap* anotherBitmap = bmp->Clone(sourceRect, PixelFormat32bppARGB);
        draw->drawImage(anotherBitmap, x , y);
    }
    
}
