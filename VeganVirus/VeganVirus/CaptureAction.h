#pragma once

#include "resource.h"
#include "Draw.h"
#include "Action.h"

#define CAPTURE_ICON_PATH L"Images\\capture.ico"
#define CAPTURE_EVERY_X_FRAMES 10000


class CaptureAction : public Action
{
public:
	// CaptureAction Constructor
	// input: progress bar requirement to activate, hinstance from WinMain
	CaptureAction(double req, HINSTANCE hinstance);

	void start();

private:
	HWND _hwnd;
	HINSTANCE _hinstance;
};