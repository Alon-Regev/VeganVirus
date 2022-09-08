#include "DesktopManager.h"

int main()
{
    DesktopManager desktopManager;

    Sleep(3000);
    for (int i = 0; i < 150; i++)
    {
        desktopManager.moveIcon(1, { 4, 4 });
        Sleep(20);
    }

    return 0;
}
