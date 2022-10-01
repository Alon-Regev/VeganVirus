#pragma once
#include "Draw.h"
#include <vector>

class Animation
{
public:
	// Animation Constructor
	// input:	
	//	draw object,
	//	time between animation frames (in seconds),
	//	paths of frame images
	Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths);

	// Animation Constructor
	// input:	
	//	draw object,
	//	time between animation frames (in seconds),
	//	paths of frame images,
	//	width and height of animation in pixels
	Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths, double w, double h);

	// Animation Destructor
	~Animation();

	// method updates the animation frames and it's position, and redraws it to the screen
	// input:	
	//	time since last update (in seconds),
	//	animation draw position (x, y) in pixels
	// return:	none
	void update(double dt, int x, int y);

	// method resets animation's current state and restarts it.
	// input:	none
	// return:	none
	void reset();

private:
	Draw* _draw;
	std::vector<Bitmap*> _bitmaps;
	double _timeBetweenFrames;
	int _currentFrame;
	double _timeUntilNext;
};

