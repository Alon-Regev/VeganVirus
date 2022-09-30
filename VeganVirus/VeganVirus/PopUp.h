#pragma once
#include "Action.h"

#define POP_UP_ICON_PATH L"pop_up.ico"
#define IMAGE_PATH L"cat.png"

class PopUp : public Action
{
public:
	PopUp(double req, Draw* draw);
	void duplicateWindow();
	void start();

private:
	int _numberOfWindows;
	Bitmap* _bmp;
	Draw* _draw;
};

