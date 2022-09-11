#include "draw_to_screen.h"

Draw::Draw(HINSTANCE hInstance)
{ 

    GdiplusStartupInput input = { 0 };
    input.GdiplusVersion = 1;
    GdiplusStartup(&this->_token, &input, NULL);

    this->createWindow(hInstance);
    //this->_graphics = new ::Graphics(GetDC(this->_hwnd));

    SetLayeredWindowAttributes(this->_hwnd, RGB(0, 0, 0), 128, LWA_COLORKEY);

    ShowWindow(this->_hwnd, SW_SHOWMAXIMIZED);

    this->_graphics = new ::Graphics(this->_hwnd, FALSE);

    this->_graphics->SetCompositingMode(CompositingModeSourceCopy);
    this->_graphics->SetCompositingQuality(CompositingQualityHighSpeed);
    this->_graphics->SetPixelOffsetMode(PixelOffsetModeNone);
    this->_graphics->SetSmoothingMode(SmoothingModeNone);
    this->_graphics->SetInterpolationMode(InterpolationModeDefault);
}

Draw::~Draw()
{
    delete this->_graphics;
    GdiplusShutdown(this->_token);
}

void Draw::createWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"Static";
    this->_hwnd = ::CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        CLASS_NAME,
        L"stop the killing",
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0,
        NULL, NULL,
        hInstance,
        NULL
    );
}

void Draw::drawImage(Bitmap* bmp, int x, int y)
{    
    this->_graphics->DrawImage(bmp, x, y);
}

void Draw::clear()
{
    SolidBrush brush(Color(0));
    this->_graphics->FillRectangle(&brush, 0, 0, 400, 400);
}
