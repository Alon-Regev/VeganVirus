#pragma once
#include <random>
#include <functional>
#include "Action.h"

#define POP_UP_ADS_ACTION_PROGRESS 0.999
#define POP_UP_ADS_ACTION_TIME 30
#define POP_UP_ADS_ACTION_ICON L"Images\\popupIcon.png"
#define CLASS_NAME L"MyFirstWindow"
#define AD_WINDOW_TITLE L"Support Animals!"
#define AD_SCREEN_MARGIN 10
#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 458

#define AD_TIMER_BACKGROUND_COLOR 20, 20, 20
#define AD_TIMER_TEXT_COLOR 240, 240, 240
#define AD_TIMER_WIDTH 130
#define AD_TIMER_HEIGHT 40
#define AD_TIMER_MARGIN 10
#define AD_TIMER_DISTANCE_FROM_BOTTOM 40

#define AD_IMAGE_COUNT 6

class PopUpAdsAction :
	public Action
{
public:
	std::vector<HBITMAP> hbitmaps;
	std::vector<HBRUSH> brushes;

	PopUpAdsAction(double req, Draw* draw);
	~PopUpAdsAction();

	// method called to start the action
	// input: none
	// return: none
	virtual void start();

	// method called to update the action every frame
	// input: time since last frame
	// return: none
	virtual void update(double dt);

	// method updates the icon position by checking if the icon is out of the screen boundaries.
	// input: icon position (POINT)
	// return: position the icon needs to be in (POINT)
	void createWindow();

	bool isActionActive();

private:

	Draw* _draw;
	double _actionTime;

};

