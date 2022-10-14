#pragma once
#include "Action.h"

#define POP_UP_ICON_PATH L"pop_up.ico"
#define POP_UP_DURATION 30
#define NUM_OF_IMAGES 17
#define IMAGE_WIDTH 255
#define IMAGE_HEIGHT 255
#define POPUP_IMAGE_PATH(n) (L"Images\\" + std::to_wstring(n) + L".png")

class ImageAction : public Action
{
public:
	ImageAction(double req, Draw* draw);
	~ImageAction();
	
	virtual void start();
	virtual void update(double dt);

private:
	// the function change the current image
	// param: none
	// return: none
	void setImage();

	// the function check if image hit side of screen
	// param: none
	// return bool: of out of bounce return true
	void updateWallBounce(double dt);

	int _x, _y;
	double _movmentX = 150, _movmentY = 150;	// pixels per second
	double _time;
	Bitmap* _bmp;
	Draw* _draw;

	int _imageNumber = 0;
};

