#pragma once
#include "Action.h"
#include <string>
#include <windows.h> 

#define CLIP_BOARD_ICON_PATH L"clipBoard.ico"

class ClipBoardAction : public Action
{
public:
	ClipBoardAction(HWND, double);
	void clipBoardWrite(std::string content);
	std::string ClipBoardRead();
	void clearClipBoard();

	void start();

private:
	HGLOBAL _hMem;
	HWND _hwnd;
};

