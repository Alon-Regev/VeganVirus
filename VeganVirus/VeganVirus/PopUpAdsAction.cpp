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
     wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
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

     this->_bitmap = (HBITMAP)LoadImageW(NULL, L"ad.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
     MessageBoxA(NULL, std::to_string(GetLastError()).c_str(), "Error", 0);
}

 PopUpAdsAction::~PopUpAdsAction()
 {
     DeleteObject(this->_bitmap);
 }

void PopUpAdsAction::start()
{
    _actionTime = POP_UP_ADS_ACTION_TIME;
    this->createWindow();
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
        DestroyWindow(hwnd);
        if (_this->isActionActive())
        {
            _this->createWindow();
            _this->createWindow();
        }
        break;
    }
    case WM_CREATE:
    {
        PopUpAdsAction* _this = (PopUpAdsAction*)((CREATESTRUCT*)lpParam)->lpCreateParams;
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps = { 0 };
        PopUpAdsAction* _this = (PopUpAdsAction*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hMemDC = CreateCompatibleDC((HDC)hdc);
        ::SelectObject(hMemDC, _this->_bitmap);
        BitBlt((HDC)hdc, 0, 0, 200, 400, hMemDC, 0, 0, SRCCOPY);
        ::DeleteDC(hMemDC);
        EndPaint(hwnd, &ps);
        break;
    }
    default:
        return DefWindowProcW(hwnd, msg, wParam, lpParam);
    }
    return 0;
}

void AdWindowMessageLoop(HWND hwnd)
{
    MSG Msg = { 0 };
    while (GetMessage(&Msg, NULL, NULL, NULL))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

int PopUpAdsAction::createWindow()
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

    //std::thread(AdWindowMessageLoop, hwnd).detach();
    AdWindowMessageLoop(hwnd);
 
    return 0;
}

bool PopUpAdsAction::isActionActive()
{
    return this->_actionTime > 0;
}
