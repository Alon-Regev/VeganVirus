#include <windows.h>
#define ICON_SIZE_REGISTRY_PATH  "Software\Microsoft\Windows\Shell\Bags\1\Desktop\IconSize"
#include <iostream>
#include<stdio.h>
void main()
{
	DWORD iconSize = 256;
	LSTATUS reg_value = RegSetValueExA(HKEY_CURRENT_USER, ICON_SIZE_REGISTRY_PATH, 0, REG_DWORD, (LPBYTE)&iconSize, sizeof(DWORD));
	

}
	