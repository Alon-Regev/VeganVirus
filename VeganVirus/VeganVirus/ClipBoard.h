#pragma once
#include "Action.h"
#include <string>
#include <windows.h> 

class ClipBoard : public Action
{
public:
	ClipBoard(HWND, double);
	void clipBoardWrite(std::string content);
	std::string ClipBoardRead();
	void clearClipBoard();

	void start();

private:
	HGLOBAL _hMem;
	HWND _hwnd;
};

