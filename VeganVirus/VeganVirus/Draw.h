#pragma once
#include "resource.h"
#include <windows.h> 
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <shobjidl_core.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

typedef void(*DrawFrameCallback)(double);

#define FPS 30

class Draw
{
public:
	// Draw Constructor
	// input: HINSTANCE of window to use
	//		  callback for drawing on screen
	Draw(HINSTANCE hInstance, DrawFrameCallback drawCallback);

	// Draw Destructor
	~Draw();

	// method draws an image on the screen
	// input: bitmap to draw, position on screen (x, y) in pixels.
	// return: none
	void drawImage(Bitmap* bmp, int x, int y);

	// method draws a rectangle on the screen
	// input: rect to draw (x, y, w , h) in pixels
	//		  color to draw (r, g, b, opt a)
	void drawRectangle(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a = 255);

	// method draws a line on the screen
	// input: line to draw (x1, y1) -> (x2, y2) in pixels,
	//		  line width,
	//		  color to draw (r, g, b, opt a)
	void drawLine(int x1, int y1, int x2, int y2, double w, BYTE r, BYTE g, BYTE b, BYTE a = 255);

	// method loads a bitmap in a specific size
	// input: bitmap path to load,
	//		  width and height in pixels
	// return: Bitmap object pointer (needs to be deleted by user)
	static Bitmap* resizedBitmap(const wchar_t* path, int w, int h);

	// method draws the current frame to the screen
	// input: none
	// return: none
	void applyFrame();

	// method gets message updates
	// input: none
	// return: whether can get message or not
	bool update();

	// method returns size of screen
	// input: none
	// return: size of screen in pixels (x, y)
	POINT getScreenSize();

	// getter for window handle (HWND)
	HWND getWindowHandle() const;
private:
	// method creates a window
	// input: hinstance to use
	// return: none
	void createWindow(HINSTANCE hInstance);

	HWND _hwnd;
	ULONG_PTR _token;

	::Bitmap* _offscreenBmp;
	::Graphics* _offscreenGraphics;
	::Graphics* _graphics;
};
