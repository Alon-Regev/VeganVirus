#include <windows.h>
#include <stdio.h>

#define ICON_SIZE_REGISTRY_PATH  "Software\\Microsoft\\Windows\\Shell\\Bags\\1\\Desktop"

void main()
{
	DWORD iconSize = 32;
    HKEY hkey = NULL;
    LSTATUS ret = RegOpenKeyExA(HKEY_CURRENT_USER, ICON_SIZE_REGISTRY_PATH, 0, KEY_SET_VALUE, &hkey);
    if (ret != ERROR_SUCCESS)
    {
        printf("Can't open registry key (Error %d)\n", ret);
        return;
    }
	ret = RegSetValueExA(hkey, "IconSize", 0, REG_DWORD, (LPBYTE)&iconSize, sizeof(DWORD));
    RegCloseKey(hkey);
    if (ret != ERROR_SUCCESS)
	{
        printf("Can't change registry value (Error %d)\n", ret);
		return;
	}

    system("tskill explorer");
}
	