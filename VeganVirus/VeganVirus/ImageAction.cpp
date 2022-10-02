#include "ImageAction.h"

//global var
int num = 0;


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

/*
move image to random location and set the time
param: none
return: none
*/
void ImageAction::start()
{
    //check that image don't start in out of bounce
    _x = GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH;//rand() % (GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH);
    _y = GetSystemMetrics(SM_CYSCREEN) - IMAGE_HIGH;// rand() % (GetSystemMetrics(SM_CYSCREEN) - IMAGE_HIGH - 1);

    _time = POP_UP_DURATION;
}

/*
the function check if to activate
param dt: difference of time between each frame
return: none
*/
void ImageAction::update(double dt)
{
    if (_time <= 0)
    {
        return;
    }
    _time -= dt;    
    
    //move(pixelPerSecond)
    _x += movmentX * dt * 15;
    _y += movmentY * dt * 15; 

    updateWallBounce(dt);

    this->_draw->drawImage(_bmp, _x, _y);
}

/*
the function change the current image
param: none
return: none
*/
void ImageAction::setImage()
{
    delete _bmp;
    num++;
    //choose random image
    std::wstring path = POPUP_IMAGE_PATH(num % NUM_OF_IMAGES);
    //convert string wchar
    _bmp = new Bitmap((wchar_t*)path.c_str());
}

/*
the function check if image hit side of screen
param: none
return bool: of out of bounce return true 
*/
void ImageAction::updateWallBounce(double dt)
{
    if (_x > GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH || _x < 0)
    {
        setImage();
        movmentX *= -1;
        //return to previous position
        _x += movmentX * dt * 15;
    }
    if (_y > GetSystemMetrics(SM_CYSCREEN) - IMAGE_HIGH || _y < 0)
    {
        setImage();
        movmentY *= -1;
        //return to previous position
        _y += movmentY * dt * 15;
    }
}
