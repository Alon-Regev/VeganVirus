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
	// input: pointer to draw object, mouse manager reference
	FruitThrow(Draw* draw, MouseManager& mouseManager);

	// FruitThrow Destructor
	~FruitThrow();

	// method updates the fruit throw object
	// input: time since last update
	// return: none
	void update(double dt);

	// method draws the fruit throw object to the screen (for one frame)
	// input: none
	// return: none
	void draw();

	// method checks if the fruit throw object collides with the mouse
	// input: mouse position (x1, y1) in pixels
	// return: true if there's a collision
	bool checkMouseCollision(double x1, double y1);

	// method computes results of collision with the mouse
	// input: mouse position and speed (x1, y1, vx1, vy1) in pixels or px/s
	// return: new mouse velocity vector as a point (vx, vy) in px/s
	DPoint_t mouseCollision(double x1, double y1, double vx1, double vy1);

	// method checks if the fruit throw object is outside the screen and ready to be deleted
	// input: none
	// return: true if out of bounds
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

