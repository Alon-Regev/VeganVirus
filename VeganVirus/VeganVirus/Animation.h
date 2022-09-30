#pragma once
#include "Draw.h"
#include <vector>

class Animation
{
public:
	Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths);
	Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths, double w, double h);

	~Animation();

	void update(double dt, int x, int y);

	void reset();

private:
	Draw* _draw;
	std::vector<Bitmap*> _bitmaps;
	double _timeBetweenFrames;
	int _currentFrame;
	double _timeUntilNext;
};

