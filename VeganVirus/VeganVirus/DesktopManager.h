#pragma once
#include <windows.h>
#include <exception>
#include <vector>
#include <commctrl.h>
#include <ShlObj.h>

class DesktopManager
{
public:
	DesktopManager();
	~DesktopManager();

	// method returns desktop icon position based on it's index
	// input: icon index (int)
	// return: icon position (POINT)
	POINT getIconPosition(int i);

	// method sets position of desktop icon based on index
	// input: icon index (int), new position (POINT)
	// return: none
	void setIconPosition(int i, POINT p);

	// method moves icon by a relative position
	// input: icon index (int), move by (POINT)
	// return: none
	void moveIcon(int i, POINT p);

	// method returns number of icons on your desktop
	// input: none
	// return: number of desktop items (int)
	int iconCount();

	// method resets the desktop manager
	// input: none
	// return: none
	void reset();

private:
	HWND _hDesktopListView;
	HANDLE _hListViewProcess;
	LPPOINT _pPointData;
};
