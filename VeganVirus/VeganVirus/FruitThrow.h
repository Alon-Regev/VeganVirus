#pragma once
#include "Draw.h"
#include "MouseManager.h"
#include <vector>

#define FRUIT_GRAVITY 500
// pixels per second^2

#define CURSOR_MASS 1
#define FRUIT_MASS 100

#define BOUNCE_FACTOR 2.5

typedef struct
{
	double x;
	double y;
} DPoint_t;

class FruitThrow
{
public:
	// FruitThrow Constructor
	// input: pointer to draw object
	FruitThrow(Draw* draw, MouseManager& mouseManager);

	// FruitThrow Destructor
	~FruitThrow();

	void update(double dt);

	void draw();

	bool checkMouseCollision(double x1, double y1);
	DPoint_t mouseCollision(double x1, double y1, double vx1, double vy1);

private:
	Draw* _draw;
	MouseManager& _mouseManager;

	double _x, _y;
	double _vx, _vy;
	const double _g = FRUIT_GRAVITY;
	const double _r = 64;

	bool _collisionLock = false;

	Bitmap* _image;

	static const std::vector<const wchar_t*> fruitImages;
};
