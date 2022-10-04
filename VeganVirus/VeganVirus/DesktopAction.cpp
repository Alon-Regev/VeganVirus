#include "DesktopAction.h"
#include <cmath>



DesktopAction::DesktopAction(Draw* draw, MouseManager& mouseManager, DesktopManager& desktopManager)
	:Action(DESKTOP_ACTION_PROGRESS, DESKTOP_ACTION_ICON), _mouseManager(mouseManager), _desktopManager(desktopManager)
{
	_draw = draw;
	_actionTime = 0;
}

void DesktopAction::start()
{
	_actionTime = DESKTOP_ACTION_TIME;
}


void DesktopAction::update(double dt)
{
	if (_actionTime < 0)
		return;
	_actionTime -= dt;
	Point_t mouseTemp = _mouseManager.getMousePosition();
	POINT mousePos = { mouseTemp.x, mouseTemp.y };
	std::vector<POINT> iconVelocityArr = iconsMoveFromIcons();
	// going over all icons, calculating their velocity and updating their position
	for (int i = 0; i < _desktopManager.iconCount(); i++)
	{
		POINT iconPosition = _desktopManager.getIconPosition(i);
		POINT iconMoveCurser = velocityBetweeenPoints(iconPosition, mousePos, CURSOR_INTERACTION_VELCITY_COEFFICIENT, 1);
		POINT iconNewPosCurser = CheckIconPos({ (long)(iconPosition.x + (iconMoveCurser.x + iconVelocityArr[i].x) * dt), (long)(iconPosition.y + (iconMoveCurser.y + iconVelocityArr[i].y) * dt)});
		_desktopManager.setIconPosition(i, iconNewPosCurser);
	}
}

std::vector<POINT> DesktopAction::iconsMoveFromIcons()
{
	int iconNum = _desktopManager.iconCount();
	std::vector<POINT> iconPositions;
	for (int i = 0; i < iconNum; i++)
	{
		iconPositions.push_back(_desktopManager.getIconPosition(i));
	}
	std::vector<POINT> iconVelocities(iconNum, { 0 });
	// lopping for calcalute the velocity of the icon base on the distance from other icons
	for (int i = 0; i < iconNum; i++)
	{
		for (int j = i + 1; j < iconNum; j++)
		{
			POINT velocity = velocityBetweeenPoints(iconPositions[i], iconPositions[j], ICON_INTERACTION_VELOCITY_COEFFICIENT, 2);
			iconVelocities[i].x += velocity.x;
			iconVelocities[i].y += velocity.y;
			iconVelocities[j].x -= velocity.x;
			iconVelocities[j].y -= velocity.y;
		}
	}
	return iconVelocities;
}

POINT DesktopAction::velocityBetweeenPoints(POINT a, POINT b, double coefficient, int power)
{
	POINT d = { a.x - b.x, a.y - b.y };
	double Dpow = pow(pow(d.x, 2) + pow(d.y, 2), (power + 1.) / 2);
	long velocityX = coefficient * d.x / Dpow;
	long velocityY = coefficient * d.x / Dpow;
	return { velocityX, velocityY };
}

POINT DesktopAction::CheckIconPos(POINT newPos)
{
	// checking if the icon is out of boundries and setting is new position
	POINT screenBoundries = _draw->getScreenSize();
	if (newPos.x < SCREEN_MARGIN)
	{
		newPos.x = SCREEN_MARGIN;
	}
	if (newPos.x > screenBoundries.x - SCREEN_MARGIN)
	{
		newPos.x = screenBoundries.x - SCREEN_MARGIN;
	}
	if (newPos.y < SCREEN_MARGIN)
	{
		newPos.y = SCREEN_MARGIN;
	}
	if (newPos.y > screenBoundries.y - SCREEN_MARGIN)
	{
		newPos.y = screenBoundries.y - SCREEN_MARGIN;
	}
	return { newPos.x, newPos.y };
}
