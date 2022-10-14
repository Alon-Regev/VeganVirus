#include "MouseManager.h"

void MouseManager::moveMouse(int dx, int dy)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	// send keyboard inputs
	SendInput(1, &input, sizeof(INPUT));
}

void MouseManager::setMousePosition(int x, int y)
{
	SetCursorPos(x, y);
}

Point_t MouseManager::getMousePosition()
{
	POINT point = { 0 };
	GetCursorPos(&point);
	return { point.x, point.y };
}
