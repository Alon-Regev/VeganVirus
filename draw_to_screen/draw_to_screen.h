#pragma once
#include "resource.h"
#include <windows.h> 
#include <objidl.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

typedef void(*DrawFrameCallback)(double);

#define FPS 40

class Draw
{
public:
	Draw(HINSTANCE hInstance, DrawFrameCallback drawCallback);
	~Draw();
	void drawImage(Bitmap* bmp, int x, int y);
	void applyFrame();
	bool update();
	
	HWND _hwnd;
	ULONG_PTR _token;

	::Bitmap* _offscreenBmp;
	::Graphics* _offscreenGraphics;
	::Graphics* _graphics;
	
private:
	void createWindow(HINSTANCE hInstance);
};

