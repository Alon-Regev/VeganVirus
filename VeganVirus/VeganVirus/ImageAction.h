#pragma once
#include "Action.h"

#define POP_UP_ICON_PATH L"pop_up.ico"
#define POP_UP_DURATION 30
#define NUM_OF_IMAGES 16
#define IMAGE_WIDTH 255
#define IMAGE_HIGH 255


class ImageAction : public Action
{
public:
	ImageAction(double req, Draw* draw);
	~ImageAction();
	
	virtual void start();
	virtual void update(double dt);

private:
	void setImage();
	void isHittingWall();
	void imageAction();

	int _x;
	int _y;
	int movmentX = 10, movmentY = 10;
	double _time;
	Bitmap* _bmp;
	Draw* _draw;
};

