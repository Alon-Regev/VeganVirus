#include "VeganProgress.h"
#include <stdlib.h>

VeganProgress::VeganProgress(Draw* draw) : _draw(draw)
{
	RECT rect = { 0 };
	GetWindowRect(draw->getWindowHandle(), &rect);
	_h = rect.bottom - 2 * _y;
	_x = rect.right - _w - _y;
}

VeganProgress::~VeganProgress()
{
	for (Action* action : _actions)
	{
		delete action;
	}
}

void VeganProgress::draw(double dt)
{
	// calc color
	BYTE r = 255 * (1 - _progress), g = 255 * _progress, b = 0;

	// draw rects
	this->_draw->drawRectangle(_x - _bw, _y - _bw, _w + 2 * _bw, _h + 2 * _bw, BORDER_COLOR);
	this->_draw->drawRectangle(_x, _y, _w, _h, BACKGROUND_COLOR);
	this->_draw->drawRectangle(_x, _y + _h * (1-_progress), _w, _h * _progress, r, g, b);
	
	// update actions
	for (Action* action : this->_actions)
	{
		action->update(dt);
	}
	this->_actionTimer += dt;
	if (this->_actionTimer >= this->_actionInterval)
	{
		this->_actionTimer -= this->_actionInterval;
		this->randomAction();
	}
}

void VeganProgress::addProgress(double amount)
{
	this->_progress += amount;

	// check immediate activations
	for (Action* action : this->_actions)
	{
		if (!action->activeFlag && this->_progress < action->getReq())
		{
			action->activeFlag = true;
			action->start();
		}
	}
}

void VeganProgress::addAction(Action* action)
{
	_actions.push_back(action);
}

void VeganProgress::randomAction()
{
	int actionIndex = rand() % this->_actions.size();
	// check if can activate action
	if (this->_progress < this->_actions[actionIndex]->getReq())
	{
		this->_actions[actionIndex]->start();
	}
}
