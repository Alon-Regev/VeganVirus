#pragma once

#include "resource.h"
#include "Draw.h"
#include "Action.h"

#define CAPTURE_ICON_PATH L"capture.ico"


class Capture : public Action
{
public:
	Capture(HINSTANCE hinstance, double req);
	void start();


private:
	HWND _hwnd;
};