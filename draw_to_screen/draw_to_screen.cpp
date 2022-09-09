

#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <SDKDDKVer.h>
#include <windows.h>

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 800

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void LoadScreen(HWND hWnd);

// Entry point
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) 
{
    const wchar_t k_WndClassName[] = L"stop the killing";

    // Register window class
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = k_WndClassName;
    ::RegisterClassExW(&wcex);




    HWND hWnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED,
        k_WndClassName,
        L"stop the killing",
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        IMAGE_WIDTH, IMAGE_HEIGHT,
        NULL, NULL,
        hInstance,
        NULL);
    // Make window semi-transparent, and mask out background color
    ::SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 128, LWA_COLORKEY);

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    // Main message loop:
    MSG msg = { 0 };
    while (::GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}



HBITMAP hBitmap = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;


    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        LoadScreen(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        return TRUE;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


void LoadScreen(HWND hWnd)
{
    RECT rect;
    HDC hdc = GetDC(hWnd);
    HBITMAP hey = (HBITMAP)LoadImage(NULL, L"C:\\ariel\\projects\\VeganVirus\\img1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    DWORD help = GetLastError();

    HBRUSH brush = CreatePatternBrush(hey);
    GetWindowRect(hWnd, &rect);
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(hWnd, hdc);
}