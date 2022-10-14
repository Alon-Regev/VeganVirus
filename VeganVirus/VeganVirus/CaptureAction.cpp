
#include "CaptureAction.h"
#include <thread>

#define MAX_LOADSTRING 100
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void				CaptureImage(HWND hWnd);

// function registers winAPI class
// input: hinstance from WinMain
// return: Class Register Identifier
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEGANVIRUS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEGANVIRUS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// Capture Window Callback
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        CaptureImage(hWnd);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// function copies screen to window
// input: window handle to draw on
// return: nonw
void CaptureImage(HWND hWnd)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
        ReleaseDC(NULL, hdcScreen);
        ReleaseDC(hWnd, hdcWindow);
    }

    // Get the client area for size calculation.
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    // This is the best stretch mode.
    SetStretchBltMode(hdcWindow, HALFTONE);

    // The source DC is the entire screen, and the destination DC is the current window (HWND).
    if (!StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcScreen,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY))
    {
        MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
    }

    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);
}

CaptureAction::CaptureAction(double req, HINSTANCE hInstance) : Action(req, CAPTURE_ICON_PATH)
{
    LoadStringW(hInstance, IDC_VEGANVIRUS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    this->_hinstance = hInstance;
}

// function handles window creation and message loop
// input: hinstance from WinMain
// return: none
void captureWindowLoop(HINSTANCE hinstance)
{
    int frame = 0;
    HWND hwnd = CreateWindowW(szWindowClass, L"we are seeing you", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hinstance, nullptr);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&frame);
    ShowWindow(hwnd, SW_SHOW);
    SetTimer(hwnd, 1, 1000 / 5, NULL);

    MSG msg;
    while (GetMessageW(&msg, hwnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CaptureAction::start()
{
    std::thread(captureWindowLoop, this->_hinstance).detach();
}
