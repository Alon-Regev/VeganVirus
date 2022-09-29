#include "FruitThrowAction.h"

FruitThrowAction::FruitThrowAction(Draw* draw, MouseManager& mouseManager) : 
	Action(FRUIT_THROW_REQ, FRUIT_THROW_ICON), _draw(draw), _mouseManager(mouseManager)
{
}

FruitThrowAction::~FruitThrowAction()
{
	for (FruitThrow* fruit : this->_currentFruits)
	{
		delete fruit;
	}
}

void FruitThrowAction::start()
{
	this->_currentFruits.push_back(new FruitThrow(this->_draw));
}

void FruitThrowAction::update(double dt)
{
	for (FruitThrow* fruit : this->_currentFruits)
	{
		fruit->update(dt);
	}
}
