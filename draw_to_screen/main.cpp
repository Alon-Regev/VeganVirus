#include "draw_to_screen.h"

Draw* draw;
Bitmap* bmp;

void drawUpdate(double dt);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{   
    draw = new Draw(hInstance, drawUpdate);
    bmp = new Bitmap(L"C:\\images\\tile1323.jpg");
    
    while (draw->update());

    delete draw;
    delete bmp;
    return 0;

}

#include <math.h>

double a = 0;
const double AV = 2;    // radian/s
void drawUpdate(double dt)
{
    a += AV * dt;
    double x = sin(a) * 500 + 520;
    double y = sin(2 * a) * 200 + 300;
    draw->drawImage(bmp, x, y);
}
