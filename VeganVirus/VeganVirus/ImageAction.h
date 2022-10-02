#pragma once
#include "Action.h"

#define POP_UP_ICON_PATH L"pop_up.ico"
#define POP_UP_DURATION 30
#define NUM_OF_IMAGES 17
#define IMAGE_WIDTH 255
#define IMAGE_HIGH 255
#define POPUP_IMAGE_PATH(n) (L"popUp\\image" + std::to_wstring(n) + L".png")

class ImageAction : public Action
{
public:
	ImageAction(double req, Draw* draw);
	~ImageAction();
	
	virtual void start();
	virtual void update(double dt);

private:
	void setImage();
	void updateWallBounce(double dt);

	int _x, _y;
	double movmentX = 10, movmentY = 10;
	double _time;
	Bitmap* _bmp;
	Draw* _draw;
};

