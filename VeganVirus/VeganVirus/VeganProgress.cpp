#include "VeganProgress.h"

VeganProgress::VeganProgress(Draw* draw) : _draw(draw)
{
}

void VeganProgress::draw()
{
	
}

void VeganProgress::addProgress(double amount)
{
	this->_progress += amount;
}
