#pragma once
#include "Action.h"

#define POP_UP_ICON_PATH L"pop_up.ico"
#define IMAGE_PATH L"popUp\\cat.png"
#define POP_UP_DURATION 30


class PopUpAction : public Action
{
public:
	PopUpAction(double req, Draw* draw);
	
	virtual void start();
	virtual void update(double dt);

private:
	void duplicateWindow();

	double _time;
	int _numberOfWindows;
	Bitmap* _bmp;
	Draw* _draw;
};

