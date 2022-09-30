#pragma once
#include "Draw.h"
#include "MouseManager.h"
#include <vector>

#define FRUIT_GRAVITY 500
// pixels per second^2

#define CURSOR_MASS 1
#define FRUIT_MASS 10

#define BOUNCE_FACTOR 2.5

#define RADIUS_RANGE 40
#define BOUNCE_MARGIN_X 4
#define BOUNCE_MARGIN_Y -4

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

	bool outOfScreen();

private:
	Draw* _draw;
	MouseManager& _mouseManager;

	double _x, _y;
	POINT _screen;
	double _vx, _vy;
	const double _g = FRUIT_GRAVITY;
	double _r = 60;

	bool _collisionLock = false;

	Bitmap* _image;

	static const std::vector<const wchar_t*> fruitImages;
};

