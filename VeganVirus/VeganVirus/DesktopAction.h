#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "DesktopManager.h"


#define DESKTOP_ACTION_PROGRESS 0.998
#define DESKTOP_ACTION_TIME 30
#define CURSOR_INTERACTION_VELCITY_COEFFICIENT 10000
#define ICON_INTERACTION_VELOCITY_COEFFICIENT 10000


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

	// method that calclate the velocity of the icon base on distance from curser
	// input: Point: two point the coeficient and the power  for the calclatesion
	// return: POINT: the velocity of the two points
	virtual POINT velocityBetweeenPoints(POINT a, POINT b, double coefficient, int power);

	// method update the icon pos and check if the icon pos is on boundaries of the screen
	// input: POINT: pos of the icon
	// return: POINT: the position of the icon need to be
	virtual POINT CheckIconPos(POINT newPos);

	// method calclate the velocity of the icon base on all the other icon 
	// input: None
	// return: vector that store the velocity of the icon base on all the other icon 
	virtual std::vector<POINT> iconsMoveFromIcons();


private:
    Draw* _draw;
    MouseManager& _mouseManager;
	DesktopManager& _desktopManager;
	double _actionTime;
};


