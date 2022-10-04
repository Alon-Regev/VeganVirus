#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "DesktopManager.h"


#define DESKTOP_ACTION_PROGRESS 0.998
#define DESKTOP_ACTION_TIME 30
#define CURSOR_INTERACTION_VELCITY_COEFFICIENT 10000
#define ICON_INTERACTION_VELOCITY_COEFFICIENT 10000
#define SCREEN_MARGIN 10
#define DESKTOP_ACTION_ICON L"desktop.ico"

class DesktopAction:
    public Action
	{
	public:
	DesktopAction(Draw* draw, MouseManager& mouseManager, DesktopManager& desktopManager);

	// method called to start the action
	// input: none
	// return: none
    virtual void start();

	// method called to update the action every frame
	// input: time since last frame
	// return: none
	virtual void update(double dt);

	// method calculates an icon's velocity based on the interaction between it's position and another point.
	// input: two positions of interacting objects, 
	// the coefficient and power to use while calculating (V=C/R^p)
	// return: velocity of interaction (POINT)
	POINT velocityBetweeenPoints(POINT a, POINT b, double coefficient, int power);

	// method updates the icon position by checking if the icon is out of the screen boundaries.
	// input: icon position (POINT)
	// return: position the icon needs to be in (POINT)
	POINT CheckIconPos(POINT newPos);

	// method calculates the velocities of all icons based on the interactions between them.
	// input: none
	// return: vector that stores total velocity of each icon
	std::vector<POINT> iconsMoveFromIcons();


	private:
    Draw* _draw;
    MouseManager& _mouseManager;
	DesktopManager& _desktopManager;
	double _actionTime;
};


