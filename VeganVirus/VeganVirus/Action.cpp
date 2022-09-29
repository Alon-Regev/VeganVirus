#include "Action.h"

Action::Action(double progressRequirement, const wchar_t* iconPath) :
	_progressRequirement(progressRequirement), _iconBitmap(Draw::resizedBitmap(iconPath, ICON_SIZE))
{
}

Action::~Action()
{
	delete _iconBitmap;
}

void Action::update(double dt)
{
}

bool Action::canActivate()
{
	return true;
}

double Action::getReq() const
{
	return this->_progressRequirement;
}

Bitmap* Action::getIconBitmap()
{
	return this->_iconBitmap;
}

