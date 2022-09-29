#include "FruitThrow.h"
#include <stdlib.h>

const std::vector<const wchar_t*> FruitThrow::fruitImages = {
	L"meat.ico",
	L"message.ico",
	L"vegetables.ico"
};

FruitThrow::FruitThrow(Draw* draw) : _draw(draw)
{
	// randomize fields
	const wchar_t* path = FruitThrow::fruitImages[rand() % FruitThrow::fruitImages.size()];
	this->_image = Draw::resizedBitmap(path, _r * 2, _r * 2);

	_x = 0;
	_y = 600;
	_vx = 350;
	_vy = -600;
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

	this->_draw->drawImage(this->_image, _x - _r, _y - _r);
}
