#include "VeganProgress.h"
#include <stdlib.h>

VeganProgress::VeganProgress(Draw* draw) : 
	_draw(draw), _meatIcon(draw->resizedBitmap(MEAT_ICON_FILE, PROGRESS_BAR_ICON_SIZE)), _vegetableIcon(draw->resizedBitmap(VEGETABLE_ICON_FILE, PROGRESS_BAR_ICON_SIZE))
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
	delete this->_meatIcon;
	delete this->_vegetableIcon;
}

void VeganProgress::draw(double dt)
{
	// calc color
	BYTE r = 255 * (1 - _progress), g = 255 * _progress, b = 0;

	// draw rects
	this->_draw->drawRectangle(_x - _bw, _y - _bw, _w + 2 * _bw, _h + 2 * _bw, BORDER_COLOR);
	this->_draw->drawRectangle(_x, _y, _w, _h, BACKGROUND_COLOR);
	this->_draw->drawRectangle(_x, _y + _h * (1-_progress), _w, _h * _progress, r, g, b);

	// draw icons
	this->_draw->drawImage(_vegetableIcon, _x + _w + _bw * 2, _y - _vegetableIcon->GetHeight() * 3. / 5);
	this->_draw->drawImage(_meatIcon, _x + _w + _bw * 2, _y + _h - _meatIcon->GetHeight() * 1. / 2);

	// draw action requirements
	for (Action* action : this->_actions)
	{
		int x1 = _x + _w, x2 = _x - 2 * _bw;
		int y1 = _y + (1 - action->getReq()) * _h;
		int y2 = y1;
		this->_draw->drawLine(x1, y1, x2 - 2, y2, 6, BORDER_COLOR);
		this->_draw->drawLine(x1, y1, x2, y2, 2, BACKGROUND_COLOR);

		Bitmap* iconBitmap = action->getIconBitmap();
		this->_draw->drawImage(iconBitmap, x2 - iconBitmap->GetWidth() - 4, y2 - iconBitmap->GetHeight() / 2);
	}
	
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
			if(action->canActivate())
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
		if(this->_actions[actionIndex]->canActivate())
			this->_actions[actionIndex]->start();
	}
}
