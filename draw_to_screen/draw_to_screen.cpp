/**/
#include <windows.h> 
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

void Example_DrawImage9(HDC hdc)
{
    Graphics graphics(hdc);
    // Create an Image object.
    Image image(L"C:\\ariel\\projects\\VeganVirus\\download.png");
    // Create a Pen object.
    Pen pen(Color(255, 255, 0, 0), 2);
    // Draw the original source image.
    graphics.DrawImage(&image, 10, 10);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input = { 0 };
    input.GdiplusVersion = 1;
    GdiplusStartup(&token, &input, NULL);

    const wchar_t CLASS_NAME[] = L"Static";
    WNDCLASS wc = {};
    wc.lpfnWndProc = &WindowProc; //attach this callback procedure
    wc.hInstance = hInstance; //handle to application instance
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc); //register wc
    // Create the window.
    HWND hwnd = ::CreateWindowExW(WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        CLASS_NAME,
        L"stop the killing",
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 300,
        NULL, NULL,
        hInstance,
        NULL);

    ::SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 128, LWA_COLORKEY);


    if (hwnd != NULL)
    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            //ShowWindow(hwnd, nCmdShow);
            //UpdateWindow(hwnd);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(token);
    return 0;

}

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
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Example_DrawImage9(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
