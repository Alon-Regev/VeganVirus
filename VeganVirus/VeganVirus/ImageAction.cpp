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

/*
move image to random location and set the time
param: none
return: none
*/
void ImageAction::start()
{
    _x = rand() % (GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH);
    _y = rand() % (GetSystemMetrics(SM_CYSCREEN) - IMAGE_HIGH);
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
    
    imageAction();
}

/*
the function change the current image
param: none
return: none
*/
void ImageAction::setImage()
{
    delete _bmp;
    int num = rand() % NUM_OF_IMAGES;
    //choose random image
    std::string path = "popUp\\image" + std::to_string(num) + ".png";
    //convert string wchar
    std::wstring wstring(path.begin(), path.end());
    _bmp = new Bitmap((wchar_t*)wstring.c_str());
}

/*
the function check if image hit side of screen
param: none
return: none
*/
void ImageAction::isHittingWall()
{
    if (_x >= GetSystemMetrics(SM_CXSCREEN) - IMAGE_WIDTH || _x < 0)
    {
        setImage();
        movmentX *= -1;
    }
    if (_y >= GetSystemMetrics(SM_CYSCREEN) - IMAGE_HIGH || _y < 0)
    {
        setImage();
        movmentY *= -1;
    }
}

/*
the function change the image location and drwa it
param: none
return: none*/
void ImageAction::imageAction()
{
    isHittingWall();

    //move
    _x += movmentX;
    _y += movmentY;

    this->_draw->drawImage(_bmp, _x, _y);
}
