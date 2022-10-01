#include "DesktopAction.h"
#include <cmath>

DesktopAction::DesktopAction(Draw* draw, MouseManager& mouseManager, DesktopManager& desktopManager)
	:Action(DESKTOP_ACTION_PROGRESS, L"meat.ico"), _mouseManager(mouseManager), _desktopManager(desktopManager)
{
	_draw = draw;
	_mouseManager = mouseManager;
	_desktopManager = desktopManager;
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
	POINT screenBoundries = _draw->getScreenSize();
	for (int i = 0; i < _desktopManager.iconCount(); i++)
	{
		POINT distancePoint = displacement(i);
		double Dsqr = pow(distancePoint.x, 2) + pow(distancePoint.y, 2);
		double iconSpeedX = SPEEDCONSTANT * distancePoint.x / Dsqr;
		float iconSpeedY = SPEEDCONSTANT * distancePoint.y / Dsqr;
		POINT movePoint = { iconSpeedX * dt, iconSpeedY * dt };

		POINT iconPos = _desktopManager.getIconPosition(i);
		POINT iconNewPos = { 0 };
		iconNewPos.x = iconPos.x + iconSpeedX * dt;
		iconNewPos.y = iconPos.y + iconSpeedY * dt;
		if (iconNewPos.x < 0)
		{
			iconNewPos.x = 0;
		}
		if (iconNewPos.x > screenBoundries.x )
		{
			iconNewPos.x = screenBoundries.x - 10;
		}
		if (iconNewPos.y < 0)
		{
			iconNewPos.y = 0;
		}
		if (iconNewPos.y > screenBoundries.y)
		{
			iconNewPos.y = screenBoundries.y - 10;
		}
		_desktopManager.setIconPosition(i, iconNewPos);
	}
}

POINT DesktopAction::displacement(int index)
{
	Point_t mousePos = _mouseManager.getMousePosition();
	POINT iconPos = _desktopManager.getIconPosition(index);
	return { iconPos.x - mousePos.x, iconPos.y - mousePos.y };
}


