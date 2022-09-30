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
	this->_fruitLeft += FRUIT_COUNT;
	this->_timer = 0;
}

void FruitThrowAction::update(double dt)
{
	// move mouse
	if (_vx != 0 || _vy != 0)
	{
		_mouseManager.moveMouse(_vx * dt, _vy * dt);
		// friction
		double mult = CURSOR_FRICTION / sqrt(_vx * _vx + _vy * _vy);
		double ux = _vx * (1 + mult), uy = _vy * (1 + mult);
		if (signbit(_vx) != signbit(ux) || signbit(_vy) != signbit(uy))
		{
			_vx = _vy = 0;
			BlockInput(FALSE);
			_mouseDisabled = false;
		}
		else
		{
			_vx = ux;
			_vy = uy;
		}
	}
	// check collisions
	for (FruitThrow* fruit : this->_currentFruits)
	{
		fruit->update(dt);
		if (fruit->checkMouseCollision())
		{
			DPoint_t u = fruit->mouseCollision(0, 0);
			_vx = u.x, _vy = u.y;
			BlockInput(TRUE);
			_mouseDisabled = true;
		}
	}
	this->_timer -= dt;
	if (this->_fruitLeft && this->_timer <= 0)
	{
		this->_currentFruits.push_back(new FruitThrow(this->_draw, this->_mouseManager));
		this->_timer += (float)rand() / RAND_MAX * (MAX_FRUIT_TIMER - MIN_FRUIT_TIMER) + MIN_FRUIT_TIMER;
		this->_fruitLeft--;
	}
}
