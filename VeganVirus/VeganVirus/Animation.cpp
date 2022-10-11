#include "Animation.h"

Animation::Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths)
{
	this->_draw = draw;
	this->_timeBetweenFrames = timeBetweenFrames;
	this->reset();

	// load bitmaps
	for (const wchar_t* framePath : frameImagePaths)
	{
		this->_bitmaps.push_back(new Bitmap(framePath));
	}
}

Animation::Animation(Draw* draw, double timeBetweenFrames, const std::vector<const wchar_t*> frameImagePaths, double w, double h)
{
	this->_draw = draw;
	this->_timeBetweenFrames = timeBetweenFrames;
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
	// update current frame timer
	this->_timeUntilNext -= dt;
	if (this->_timeUntilNext <= 0)
	{
		// move to next frame
		this->_timeUntilNext += this->_timeBetweenFrames;
		this->_currentFrame = (this->_currentFrame + 1) % this->_bitmaps.size();
	}
	
	// draw frame
	this->_draw->drawImage(this->_bitmaps[this->_currentFrame], x, y);
}

void Animation::reset()
{
	this->_timeUntilNext = this->_timeBetweenFrames;
	this->_currentFrame = 0;
}
