#include "PopUpAdsAction.h"
#include <windows.h>
#include <stdlib.h>  
#include <thread>

LRESULT CALLBACK AdWindowCallBack(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


 PopUpAdsAction::PopUpAdsAction(Draw* draw)
    :Action(POP_UP_ADS_ACTION_PROGRESS, POP_UP_ADS_ACTION_ICON)
{
     _draw = draw;
     _actionTime = 0;

     WNDCLASSW wc = { 0 };
     wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
     wc.hCursor = LoadCursor(NULL, IDC_ARROW);
     wc.lpszClassName = CLASS_NAME;
     wc.lpfnWndProc = AdWindowCallBack;
     wc.hInstance = GetModuleHandle(NULL);
     wc.style = CS_HREDRAW | CS_VREDRAW;

     if (!RegisterClassW(&wc))
     {
         MessageBoxA(NULL, std::to_string(GetLastError()).c_str(), "GetLastError", 0);
         MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
     }

     this->hbitmap = (HBITMAP)LoadImageW(NULL, L"ad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
     this->brush = CreatePatternBrush(this->hbitmap);
}

 PopUpAdsAction::~PopUpAdsAction()
 {
     DeleteObject(this->brush);
     DeleteObject(this->hbitmap);
 }

void PopUpAdsAction::start()
{
    _actionTime = POP_UP_ADS_ACTION_TIME;
    std::thread(&PopUpAdsAction::createWindow, this).detach();
}

void PopUpAdsAction::update(double dt)
{
    if (_actionTime <= 0)
        return;
    _actionTime -= dt;
}

LRESULT CALLBACK AdWindowCallBack(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lpParam)
{
    static thread_local HDC* phdc = nullptr;
    if (!phdc)
    {
        phdc = new HDC;
        *phdc = GetDC(hwnd);
    }
    switch (msg)
    {
    case WM_CLOSE:
    {
        PopUpAdsAction* _this = (PopUpAdsAction*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (_this->isActionActive())
        {
            std::thread(&PopUpAdsAction::createWindow, _this).detach();
            std::thread(&PopUpAdsAction::createWindow, _this).detach();
        }
        DestroyWindow(hwnd);
        ReleaseDC(hwnd, *phdc);
        delete phdc;
        break;
    }
    case WM_PAINT:
    {
        PopUpAdsAction* _this = (PopUpAdsAction*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        RECT rect;
        GetWindowRect(hwnd, &rect);
        rect.right -= rect.left;
        rect.left = 0;
        rect.bottom -= rect.top;
        rect.top = 0;
        FillRect(*phdc, &rect, _this->brush);
        break;
    }
    default:
        return DefWindowProcW(hwnd, msg, wParam, lpParam);
    }
    return 0;
}

int intRand(const int& min, const int& max)
{
    static thread_local std::mt19937* generator = nullptr;
    if (!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}

void PopUpAdsAction::createWindow()
{
    HWND hwnd;
    POINT screenSize = _draw->getScreenSize();
    int maxX = screenSize.x - SCREEN_WIDTH - AD_SCREEN_MARGIN;
    int maxY = screenSize.y - AD_SCREEN_MARGIN - SCREEN_HEIGHT;
    int randX = intRand(0, 1e6) % (maxX - AD_SCREEN_MARGIN) + AD_SCREEN_MARGIN;
    int randY = intRand(0, 1e6) % (maxY - AD_SCREEN_MARGIN) + AD_SCREEN_MARGIN;

    hwnd = CreateWindowW(CLASS_NAME, L"MyWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, randX, randY, SCREEN_WIDTH, SCREEN_HEIGHT
        ,NULL, NULL, NULL, this);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    if (!hwnd)
    {
        MessageBoxA(NULL, std::to_string(GetLastError()).c_str(), "GetLastError", 0);
    }

    MSG Msg = { 0 };
    while (GetMessageW(&Msg, hwnd, NULL, NULL))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

bool PopUpAdsAction::isActionActive()
{
    return this->_actionTime > 0;
}
