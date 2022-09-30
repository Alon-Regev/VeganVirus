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

	// set previous mouse position
	Point_t point = _mouseManager.getMousePosition();
	_px = point.x;
	_py = point.y;
}

void FruitThrowAction::update(double dt)
{
	double x1 = _px, y1 = _py;

	if (!_mouseDisabled)
	{
		// get new mouse position
		Point_t point = _mouseManager.getMousePosition();
		// calc velocity
		_vx = (point.x - _px) / dt;
		_vy = (point.y - _py) / dt;
	}

	// compute subframes
	double t = dt;
	for (; t > COMPUTATION_SUB_FRAME_INTERVAL; t -= COMPUTATION_SUB_FRAME_INTERVAL)
	{
		x1 += _vx * COMPUTATION_SUB_FRAME_INTERVAL;
		y1 += _vy * COMPUTATION_SUB_FRAME_INTERVAL;
		this->_draw->drawRectangle(x1, y1, 6, 6, 255, 0, 0);
		this->subFrameUpdate(COMPUTATION_SUB_FRAME_INTERVAL, x1, y1);
	}
	x1 += _vx * t;
	y1 += _vy * t;
	this->subFrameUpdate(t, x1, y1);

	_px = x1;
	_py = y1;

	if (_mouseDisabled)
	{
		_mouseManager.setMousePosition(x1, y1);
	}

	for (FruitThrow* fruit : this->_currentFruits)
	{
		fruit->draw();
	}

	this->_timer -= dt;
	if (this->_fruitLeft && this->_timer <= 0)
	{
		this->_currentFruits.push_back(new FruitThrow(this->_draw, this->_mouseManager));
		this->_timer += (float)rand() / RAND_MAX * (MAX_FRUIT_TIMER - MIN_FRUIT_TIMER) + MIN_FRUIT_TIMER;
		this->_fruitLeft--;
	}
}

void FruitThrowAction::subFrameUpdate(double dt, double x1, double x2)
{
	// move mouse
	if (_vx != 0 || _vy != 0)
	{
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
		if (fruit->checkMouseCollision(x1, x2))
		{
			DPoint_t u = fruit->mouseCollision(x1, x2, _vx / 5, _vy / 5);
			_vx = u.x, _vy = u.y;
			BlockInput(TRUE);
			_mouseDisabled = true;
		}
	}
}
