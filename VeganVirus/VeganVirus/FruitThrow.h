#pragma once
#include "Draw.h"
#include <vector>

#define FRUIT_GRAVITY 500
// pixels per second^2

class FruitThrow
{
public:
	// FruitThrow Constructor
	// input: pointer to draw object
	FruitThrow(Draw* draw);

	// FruitThrow Destructor
	~FruitThrow();

	void update(double dt);

private:
	Draw* _draw;

	int _x, _y;
	int _vx, _vy;
	const int _g = FRUIT_GRAVITY;
	const int _r = 64;

	Bitmap* _image;

	static const std::vector<const wchar_t*> fruitImages;
};

