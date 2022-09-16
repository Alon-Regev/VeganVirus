#pragma once
#include <string>
#include <windows.h> 

class ClipBoard
{
public:
	ClipBoard(HWND);
	~ClipBoard();

	void clipBoardWrite(std::string content);
	std::string ClipBoardRead();
	void clearClipBoard();

private:
	HGLOBAL _hMem;
	HWND _hwnd;
};

