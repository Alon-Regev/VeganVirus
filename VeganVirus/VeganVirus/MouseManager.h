#pragma once
#include <Windows.h>

typedef struct
{
	int x;
	int y;
} Point_t;

class MouseManager
{
public:
	// method moves the mouse relative to the current position
	// input: relative position to move (dx, dy) in pixels
	// return: none
	void moveMouse(int dx, int dy);

	// method moves the mouse to an absolute position in screen coordinates
	// input: position to move to (x, y) in pixels
	// return: none
	void setMousePosition(int x, int y);

	// method returns the current mouse position in pixels (relative to the top-left corner of the screen)
	// input: none
	// return: Point struct of ints (x, y)
	Point_t getMousePosition();
};
