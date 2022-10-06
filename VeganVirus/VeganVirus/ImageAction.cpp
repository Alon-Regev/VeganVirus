#include "ImageAction.h"

ImageAction::ImageAction(double req, Draw* draw) :
    Action(req, POP_UP_ICON_PATH), _draw(draw)
{
    _time = 0;
    _bmp = new Bitmap(L"popUp\\image0.png");
}

ImageAction::~ImageAction()
{
    delete _bmp;
}

void ImageAction::start()
{
    //randomize location
    _x = rand() % (GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH);
    _y = rand() % (GetSystemMetrics(SM_CYSCREEN) - IMAGE_HEIGHT);

    _time = POP_UP_DURATION;
}

void ImageAction::update(double dt)
{
    if (_time <= 0)
    {
        return;
    }
    _time -= dt;    
    
    // update position
    _x += _movmentX * dt;
    _y += _movmentY * dt; 

    updateWallBounce(dt);

    this->_draw->drawImage(_bmp, _x, _y);
}

void ImageAction::setImage()
{
    delete _bmp;
    this->_imageNumber++;
    //choose random image
    std::wstring path = POPUP_IMAGE_PATH(this->_imageNumber % NUM_OF_IMAGES);
    //convert string wchar
    _bmp = new Bitmap((wchar_t*)path.c_str());
}

void ImageAction::updateWallBounce(double dt)
{
    if (_x > GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH || _x < 0)
    {
        setImage();
        _movmentX *= -1;
        //return to previous position
        _x += _movmentX * dt;
    }
    if (_y > GetSystemMetrics(SM_CYSCREEN) - IMAGE_HEIGHT || _y < 0)
    {
        setImage();
        _movmentY *= -1;
        //return to previous position
        _y += _movmentY * dt;
    }
}
