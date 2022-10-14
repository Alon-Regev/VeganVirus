#include "PopUpAdsAction.h"
#include <windows.h>
#include <stdlib.h>  
#include <thread>

LRESULT CALLBACK AdWindowCallBack(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


 PopUpAdsAction::PopUpAdsAction(double req, Draw* draw)
    :Action(req, POP_UP_ADS_ACTION_ICON)
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

     for (int i = 0; i < AD_IMAGE_COUNT; i++)
     {
         std::wstring path = L"Images\\ad" + std::to_wstring(i) + L".bmp";
         HBITMAP hbmp = (HBITMAP)LoadImageW(NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
         this->hbitmaps.push_back(hbmp);
         this->brushes.push_back(CreatePatternBrush(this->hbitmaps[i]));
     }
}

 PopUpAdsAction::~PopUpAdsAction()
 {
     for (int i = 0; i < AD_IMAGE_COUNT; i++)
     {
         DeleteObject(this->brushes[i]);
         DeleteObject(this->hbitmaps[i]);
     }
 }

 int intRand(const int& min, const int& max)
 {
     static thread_local std::mt19937* generator = nullptr;
     if (!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
     std::uniform_int_distribution<int> distribution(min, max);
     return distribution(*generator);
 }

void PopUpAdsAction::start()
{
    this->_actionTime = POP_UP_ADS_ACTION_TIME;
    std::thread(&PopUpAdsAction::createWindow, this).detach();
}

void PopUpAdsAction::update(double dt)
{
    if (this->_actionTime <= 0)
        return;
    this->_actionTime -= dt;
    // show timer
    POINT screen = this->_draw->getScreenSize();
    std::wstring toWrite = L"Skip Ads in " + std::to_wstring((int)this->_actionTime + 1);

    int y = screen.y - AD_TIMER_HEIGHT - AD_TIMER_DISTANCE_FROM_BOTTOM;
    this->_draw->drawRectangle(0, y, AD_TIMER_WIDTH, AD_TIMER_HEIGHT, AD_TIMER_BACKGROUND_COLOR);
    this->_draw->drawText(AD_TIMER_MARGIN, y + AD_TIMER_MARGIN, toWrite.c_str(), AD_TIMER_TEXT_COLOR);
}

LRESULT CALLBACK AdWindowCallBack(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lpParam)
{
    static thread_local HDC* phdc = nullptr;
    if (!phdc)
    {
        phdc = new HDC;
        *phdc = GetDC(hwnd);
    }
    static thread_local int* pImageIndex = nullptr;
    if (!pImageIndex)
    {
        pImageIndex = new int;
        *pImageIndex = intRand(0, AD_IMAGE_COUNT - 1);
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
        delete pImageIndex;
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
        FillRect(*phdc, &rect, _this->brushes[*pImageIndex]);
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
    POINT screenSize = this->_draw->getScreenSize();
    int randX = intRand(AD_SCREEN_MARGIN, screenSize.x - SCREEN_WIDTH - AD_SCREEN_MARGIN);
    int randY = intRand(AD_SCREEN_MARGIN, screenSize.y - SCREEN_HEIGHT - AD_SCREEN_MARGIN);

    hwnd = CreateWindowW(CLASS_NAME, AD_WINDOW_TITLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE, randX, randY, SCREEN_WIDTH, SCREEN_HEIGHT
        ,NULL, NULL, NULL, this);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    if (!hwnd)
    {
        MessageBoxA(NULL, std::to_string(GetLastError()).c_str(), "GetLastError", 0);
    }

    MSG Msg = { 0 };
    BOOL ret;
    while (ret = GetMessageW(&Msg, hwnd, NULL, NULL))
    {
        if (ret == -1)   // error
            break;
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

bool PopUpAdsAction::isActionActive()
{
    return this->_actionTime > 0;
}
