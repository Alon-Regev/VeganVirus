#pragma once
#include <random>
#include <functional>
#include "Action.h"

#define POP_UP_ADS_ACTION_PROGRESS 0.999
#define POP_UP_ADS_ACTION_TIME 30
#define POP_UP_ADS_ACTION_ICON L"desktop.ico"
#define CLASS_NAME L"MyFirstWindow"
#define AD_SCREEN_MARGIN 20
#define SCREEN_WIDTH 400   
#define SCREEN_HEIGHT 400 

class PopUpAdsAction :
	public Action
{
public:
	HBITMAP hbitmap;
	HBRUSH brush;

	PopUpAdsAction(Draw* draw);
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

