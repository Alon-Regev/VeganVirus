#include "FruitThrow.h"
#include <stdlib.h>
#include <math.h>

const std::vector<const wchar_t*> FruitThrow::fruitImages = {
	L"Images\\fruit0.png",
	L"Images\\fruit1.png",
	L"Images\\fruit2.png",
	L"Images\\fruit3.png",
	L"Images\\fruit4.png",
	L"Images\\fruit5.png",
	L"Images\\fruit6.png",
	L"Images\\fruit7.png",
	L"Images\\fruit8.png",
};

FruitThrow::FruitThrow(Draw* draw, MouseManager& mouseManager) : _draw(draw), _mouseManager(mouseManager)
{
	// randomize fields
	_r += rand() % RADIUS_RANGE;

	const wchar_t* path = FruitThrow::fruitImages[rand() % FruitThrow::fruitImages.size()];
	this->_image = Draw::resizedBitmap(path, _r * 2, _r * 2);

	_screen = draw->getScreenSize();
	_x = (float)rand() / RAND_MAX * (_screen.x + 400) - 200;
	_y = rand() % 3 ? _screen.y + 100 : -100;
	_vx = (_screen.x / 2 - _x) * ((float)rand() / RAND_MAX * 0.5 + 0.4);
	if (_y < 0)
		_vy = 0;
	else
		_vy = -sqrt(2. * _g * _screen.y * ((float)rand() / RAND_MAX * 0.55 + 0.6));
}

FruitThrow::~FruitThrow()
{
	delete this->_image;
}

void FruitThrow::update(double dt)
{
	_x += _vx * dt;
	_y += _vy * dt;
	_vy += _g * dt;

	// check wall bounce
	if (_x < (_r - BOUNCE_MARGIN_X) && _vx < 0 || _x > _screen.x - (_r - BOUNCE_MARGIN_X) && _vx > 0)
		_vx *= -1;
	if (_y < (_r - BOUNCE_MARGIN_Y) && _vy < 0)
		_vy *= -1;
}

void FruitThrow::draw()
{
	this->_draw->drawImage(this->_image, _x - _r, _y - _r);
}

bool FruitThrow::checkMouseCollision(double x1, double y1)
{
	double x2 = _x, y2 = _y;
	double Dx = x1 - x2, Dy = y1 - y2;
	double Dsqr = Dx * Dx + Dy * Dy;
	if (Dsqr <= _r * _r)
	{
		// there's a collision
		if (this->_collisionLock)
			return false;
		this->_collisionLock = true;
		return true;
	}
	// no collision
	this->_collisionLock = false;
	return false;
}

DPoint_t FruitThrow::mouseCollision(double x1, double y1, double vx1, double vy1)
{
	double x2 = _x, y2 = _y;
	double Dx = x1 - x2, Dy = y1 - y2;
	double Dsqr = Dx * Dx + Dy * Dy;

	double vx2 = _vx, vy2 = _vy;
	double m1 = CURSOR_MASS, m2 = FRUIT_MASS;

	double a = BOUNCE_FACTOR * 2. * (Dx * (vx2 - vx1) + Dy * (vy2 - vy1)) / Dsqr / (1 / m1 + 1 / m2);

	double ux1 = vx1 + a * Dx / m1;
	double uy1 = vy1 + a * Dy / m1;
	double ux2 = vx2 - a * Dx / m2;
	double uy2 = vy2 - a * Dy / m2;

	_vx = ux2; 
	_vy = uy2;
	return { ux1, uy1 };
}

bool FruitThrow::outOfScreen()
{
	return (_y > _screen.y + _r) && (_vy > 0);
}
