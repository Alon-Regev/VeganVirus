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
}

 PopUpAdsAction::~PopUpAdsAction()
 {
     DeleteObject(this->_brush);
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
        break;
    }
    case WM_PAINT:
    {
        PopUpAdsAction* _this = (PopUpAdsAction*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        RECT rect;
        HDC hdc = GetDC(hwnd);
        HBITMAP hbitmap = (HBITMAP)LoadImageW(NULL, L"ad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        HBRUSH brush = CreatePatternBrush(hbitmap);
        GetWindowRect(hwnd, &rect);
        rect.right -= rect.left;
        rect.left = 0;
        rect.bottom -= rect.top;
        rect.top = 0;
        FillRect(hdc, &rect, brush);
        ReleaseDC(hwnd, hdc);
        break;
    }
    default:
        return DefWindowProcW(hwnd, msg, wParam, lpParam);
    }
    return 0;
}

void PopUpAdsAction::createWindow()
{
    HWND hwnd;
    POINT screenSize = _draw->getScreenSize();
    int maxX = screenSize.x - SCREEN_WIDTH - AD_SCREEN_MARGIN;
    int maxY = screenSize.y - AD_SCREEN_MARGIN - SCREEN_HEIGHT;
    int randX = rand() % (maxX - AD_SCREEN_MARGIN) + AD_SCREEN_MARGIN;
    int randY = rand() % (maxY - AD_SCREEN_MARGIN) + AD_SCREEN_MARGIN;

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
