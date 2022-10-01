#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "DesktopManager.h"


#define DESKTOP_ACTION_PROGRESS 0.998
#define DESKTOP_ACTION_TIME 40
#define SPEEDCONSTANT 3000
//	px^2 / sec 

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

	// method that calclate the distance between desktop icon to curser
	// input: int: index of the icon 
	// return: POINT: the distanc
	virtual POINT displacement(int index);

private:
    Draw* _draw;
    MouseManager& _mouseManager;
	DesktopManager& _desktopManager;
	double _actionTime;
};


