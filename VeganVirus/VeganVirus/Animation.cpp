#include "Animation.h"

Animation::Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths)
{
	_draw = draw;
	_timeBetweenFrames = timeBetweenFrames;
	this->reset();

	// load bitmaps
	for (const wchar_t* framePath : frameImagePaths)
	{
		this->_bitmaps.push_back(new Bitmap(framePath));
	}
}

Animation::Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths, double w, double h)
{
	_draw = draw;
	_timeBetweenFrames = timeBetweenFrames;
	this->reset();

	// load bitmaps
	for (const wchar_t* framePath : frameImagePaths)
	{
		this->_bitmaps.push_back(Draw::resizedBitmap(framePath, w, h));
	}
}

Animation::~Animation()
{
	for (Bitmap* bmp : this->_bitmaps)
	{
		delete bmp;
	}
}

void Animation::update(double dt, int x, int y)
{
	_timeUntilNext -= dt;
	if (_timeUntilNext <= 0)
	{
		_timeUntilNext += _timeBetweenFrames;
		this->_currentFrame = (this->_currentFrame + 1) % this->_bitmaps.size();
	}
	
	// draw frame
	this->_draw->drawImage(this->_bitmaps[this->_currentFrame], x, y);
}

void Animation::reset()
{
	_timeUntilNext = _timeBetweenFrames;
	_currentFrame = 0;
}
