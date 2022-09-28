#include "Action.h"

Action::Action(double progressRequirement, const char* iconPath) :
	_progressRequirement(progressRequirement), _iconPath(iconPath)
{
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

const char* Action::getIconPath() const
{
	return this->_iconPath;
}
