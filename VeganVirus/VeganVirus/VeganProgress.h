#pragma once
#include "Draw.h"

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

	Draw* _draw;
};
