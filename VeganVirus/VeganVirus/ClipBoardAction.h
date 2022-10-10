#pragma once
#include "Action.h"
#include <string>
#include <windows.h> 

#define CLIP_BOARD_ICON_PATH L"clipBoard.ico"

class ClipBoardAction : public Action
{
public:
	ClipBoardAction(double req);

	virtual void start();
	virtual void update(double dt);

	void clipBoardWrite(std::string content);
	std::string ClipBoardRead();

private:
	HGLOBAL _hMem;
};
