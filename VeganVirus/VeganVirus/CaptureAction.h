#pragma once

#include "resource.h"
#include "Draw.h"
#include "Action.h"

#define CAPTURE_ICON_PATH L"Images\\capture.ico"


class CaptureAction : public Action
{
public:
	CaptureAction(double req, HINSTANCE hinstance);
	void start();


private:
	HWND _hwnd;
};