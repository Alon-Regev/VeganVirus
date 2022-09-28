#pragma once
#include "Draw.h"

#define CONTENT_COLOR 0, 255, 40
#define BACKGROUND_COLOR 80, 80, 80
#define BORDER_COLOR 5, 5, 5

class VeganProgress
{
public:
	// Constructor
	// input: pointer to a draw object
	VeganProgress(Draw* draw);

	// method draws the progress bar on screen
	// input: none
	// return: none
	void draw();

	// method changes progress by given amount
	// amount: what value to add to the progress
	// return: none
	void addProgress(double amount);
private:
	double _progress = 1;
	int _x, _h;
	const int _w = 20, _y = 40;
	const int _bw = 3;	// border width

	Draw* _draw;
};
