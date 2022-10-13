#pragma once
#include <list>
#include "Action.h"
#include "MouseManager.h"
#include "DesktopManager.h"
#include <mutex>


#define DESKTOP_ACTION_PROGRESS 0.5
#define DESKTOP_ACTION_TIME 30
#define CURSOR_INTERACTION_VELCITY_COEFFICIENT 10000
#define ICON_INTERACTION_VELOCITY_COEFFICIENT 100000
#define SCREEN_MARGIN 10
#define DESKTOP_ACTION_ICON L"desktopAction.ico"
#define MAX_VELOCITY 500
#define SPEECH_BUBBLE_CHANCE_INV 100
#define SPEECH_BUBBLE_TIMER 5
#define SPEECH_BUBBLE_SIZE 320, 210

#define ICON_SIZE_REGISTRY_PATH  "Software\\Microsoft\\Windows\\Shell\\Bags\\1\\Desktop"
#define DESKTOP_FFLAGS_DISABLE_AUTO_ARRANGE_AND_GRID_ALIGN 1075839520 

typedef struct
{
	double timer;
	int iconIndex;
	const wchar_t* message;
} SpeechBubble;

class DesktopAction :
	public Action
{
public:
	DesktopAction(double req, Draw* draw, MouseManager& mouseManager, DesktopManager& desktopManager);
	~DesktopAction();

	// method called to start the action
	// input: none
	// return: none
	virtual void start();

	// method called to update the action every frame
	// input: time since last frame
	// return: none
	virtual void update(double dt);

	// method allows or blocks updates and the desktop from being accessed.
	// input: whether to allow (true) or to block (false)
	// return: none
	void allowUpdates(bool allow);

private:
	// method calculates an icon's velocity based on the interaction between it's position and another point.
	// input: two positions of interacting objects, 
	//        the coefficient and power to use while calculating (V=C/R^p)
	// return: velocity of interaction (POINT)
	POINT velocityBetweeenPoints(POINT a, POINT b, double coefficient, int power);

	// method updates the icon position by checking if the icon is out of the screen boundaries.
	// input: icon position (POINT)
	// return: position the icon needs to be in (POINT)
	POINT correctIconOutOfScreen(POINT newPos);

	// method calculates the velocities of all icons based on the interactions between them.
	// input: none
	// return: vector that stores total velocity of each icon
	std::vector<POINT> computeIconInteractionVelocities();

	// method limits the magnitude of the input velocity
	// input: velocity as POINT
	// return: corrected velocity
	POINT limitVelocity(POINT velocity);

	// method updates value in/from the registry at the start of the action
	// input: none
	// return: none
	void registryUpdate();

	Draw* _draw;
	MouseManager& _mouseManager;
	DesktopManager& _desktopManager;
	double _actionTime;

	std::vector<POINT> _iconPositions;
	int _iconSize = 32;
	std::list<SpeechBubble> _speechBubbles;
	static const std::vector<const wchar_t*> _messages;

	Bitmap* _speechBubbleBmp;

	std::mutex _allowUpdatesMutex;
	bool _allowUpdates = true;
};


