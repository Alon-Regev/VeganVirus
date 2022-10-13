#include "Draw.h"
#include <chrono>

auto t = std::chrono::high_resolution_clock::now();
//callback procedure for this window, takes in all the window details
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        return TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        Draw* draw = (Draw*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        auto nt = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(nt - t).count();
        t = nt;
        draw->drawFrameUpdate(dt);
        draw->applyFrame();
        break;
    }
    case WM_TIMER:
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        break;
    default:
        break;
    }


    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Draw::Draw(HINSTANCE hInstance, DrawFrameCallback drawCallback)
{
    this->drawFrameUpdate = drawCallback;

    const wchar_t CLASS_NAME[] = L"Static";
    WNDCLASS wc = {};
    wc.lpfnWndProc = &WindowProc; //attach this callback procedure
    wc.hInstance = hInstance; //handle to application instance
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc); //register wc

    GdiplusStartupInput input = { 0 };
    input.GdiplusVersion = 1;
    GdiplusStartup(&this->_token, &input, NULL);

    this->createWindow(hInstance);
    this->removeFromTaskBar();

    // set frame timer
    SetTimer(this->_hwnd, 1, 1000 / FPS, NULL);

    SetLayeredWindowAttributes(this->_hwnd, RGB(0, 0, 0), 128, LWA_COLORKEY);

    ShowWindow(this->_hwnd, SW_SHOWMAXIMIZED);

    this->_graphics = new ::Graphics(this->_hwnd, FALSE);

    this->_graphics->SetCompositingMode(CompositingModeSourceCopy);
    this->_graphics->SetCompositingQuality(CompositingQualityHighSpeed);
    this->_graphics->SetPixelOffsetMode(PixelOffsetModeNone);
    this->_graphics->SetSmoothingMode(SmoothingModeNone);
    this->_graphics->SetInterpolationMode(InterpolationModeDefault);

    Rect rect;
    GetClientRect(this->_hwnd, (LPRECT)&rect);
    this->_offscreenBmp = new Bitmap(rect.Width, rect.Height, PixelFormat32bppARGB);
    this->_offscreenGraphics = new ::Graphics(this->_offscreenBmp);
}

Draw::~Draw()
{
    delete this->_graphics;
    delete this->_offscreenBmp;
    delete this->_offscreenGraphics;
    GdiplusShutdown(this->_token);
}

void Draw::createWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"Static";
    this->_hwnd = ::CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        CLASS_NAME,
        L"Canvas Window",
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0,
        NULL, NULL,
        hInstance,
        NULL
    );
    SetWindowLongPtr(this->_hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

void Draw::drawImage(Bitmap* bmp, int x, int y)
{
    this->_offscreenGraphics->DrawImage(bmp, x, y);
}

void Draw::drawRectangle(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
    SolidBrush brush(Color(a, r, g, b));
    this->_offscreenGraphics->FillRectangle(&brush, x, y, w, h);
}

void Draw::drawLine(int x1, int y1, int x2, int y2, double w, BYTE r, BYTE g, BYTE b, BYTE a)
{
    Pen pen(Color(a, r, b, g), w);
    this->_offscreenGraphics->DrawLine(&pen, Point(x1, y1), Point(x2, y2));
}

void Draw::drawText(int x, int y, const wchar_t* text)
{
    Font font(&FontFamily(L"Arial"), 12);
    SolidBrush brush(Color(5, 5, 5));
    this->_offscreenGraphics->DrawString(text, -1, &font, PointF(x, y), &brush);
}

Bitmap* Draw::resizedBitmap(const wchar_t* path, int w, int h)
{
    Bitmap original(path);
    Bitmap* newBitmap = new Bitmap(w, h);
    Graphics g(newBitmap);

    float horizontalScalingFactor = (float)w / original.GetWidth();
    float verticalScalingFactor = (float)h / original.GetHeight();

    g.ScaleTransform(horizontalScalingFactor, verticalScalingFactor);
    g.DrawImage(&original, 0, 0);

    return newBitmap;
}

POINT Draw::getScreenSize()
{
    RECT rect;
    GetClientRect(this->_hwnd, &rect);
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void Draw::applyFrame()
{
    // draw offscreen graphics to screen 
    this->_graphics->DrawImage(this->_offscreenBmp, 0, 0);
    // replace bmp
    this->_offscreenGraphics->Clear(Color(0));
}

bool Draw::update()
{
    MSG msg;
    if (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
    return false;
}

HWND Draw::getWindowHandle() const
{
    return this->_hwnd;
}

void Draw::removeFromTaskBar()
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
        pTaskList->DeleteTab(this->_hwnd);
        pTaskList->Release();
    }
}
