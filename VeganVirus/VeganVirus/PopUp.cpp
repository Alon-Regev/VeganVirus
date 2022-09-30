#include "PopUp.h"



PopUp::PopUp(double req, Draw* draw) : 
    Action(req, POP_UP_ICON_PATH), _draw(draw)
{
    _bmp = new Bitmap(IMAGE_PATH);
    start();
}

void PopUp::start()
{
    _numberOfWindows = 4;
    duplicateWindow();
}


void PopUp::duplicateWindow()
{
    int x = 0, y = 0;
    RectF sourceRect(
        0.0f,
        0.0f,
        (REAL)_bmp->GetWidth(),
        (REAL)_bmp->GetHeight());


    for (int i = 0; i < _numberOfWindows; i++)
    {
        x = x > GetSystemMetrics(SM_CXSCREEN) ? 0 : x + 60;
        y = y > GetSystemMetrics(SM_CYSCREEN) ? 0 : y + 60;
        
        Bitmap* anotherBitmap = _bmp->Clone(sourceRect, PixelFormat32bppARGB);

        this->_draw->drawImage(anotherBitmap, x , y);
        delete anotherBitmap;
    }
}