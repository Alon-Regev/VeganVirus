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
}

Draw::~Draw()
{
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


void Draw::drawImage(const std::wstring path, int x, int y)
{    
    ::Graphics graphics(GetDC(this->_hwnd));
    // Create an Image object.
    ::Image image(path.c_str());
    // Draw the original source image.
    graphics.DrawImage(&image, x, y);

}

void Draw::clear()
{
    ::Graphics graphics(GetDC(this->_hwnd));
    graphics.Clear(::Color(0));
}
