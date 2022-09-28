#include "VeganProgress.h"

VeganProgress::VeganProgress(Draw* draw) : _draw(draw)
{
	RECT rect = { 0 };
	GetWindowRect(draw->getWindowHandle(), &rect);
	_h = rect.bottom - 2 * _y;
	_x = rect.right - _w - _y;
}

void VeganProgress::draw()
{
	// calc color
	BYTE r = 255 * (1 - _progress), g = 255 * _progress, b = 0;

	// draw rects
	this->_draw->drawRectangle(_x - _bw, _y - _bw, _w + 2 * _bw, _h + 2 * _bw, BORDER_COLOR);
	this->_draw->drawRectangle(_x, _y, _w, _h, BACKGROUND_COLOR);
	this->_draw->drawRectangle(_x, _y + _h * (1-_progress), _w, _h * _progress, r, g, b);
}

void VeganProgress::addProgress(double amount)
{
	this->_progress += amount;
}
