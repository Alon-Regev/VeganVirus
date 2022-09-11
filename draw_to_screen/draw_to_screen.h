#pragma once

#include "resource.h"
#include <windows.h> 
#include <objidl.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")


class Draw
{
public:
	Draw(HINSTANCE hInstance);
	~Draw();
	void drawImage(Bitmap* bmp, int x, int y);
	void clear();
	
	HWND _hwnd;
	ULONG_PTR _token;
	::Graphics* _graphics;
	::Bitmap* _bitmap;
	
private:
	void createWindow(HINSTANCE hInstance);
};

