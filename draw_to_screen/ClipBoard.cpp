#include "ClipBoard.h"


ClipBoard::ClipBoard(HWND hwnd) : _hwnd(hwnd)
{
	OpenClipboard(hwnd);
}

ClipBoard::~ClipBoard()
{
	CloseClipboard();
}

void ClipBoard::clipBoardWrite(std::string content)
{
	const size_t len = content.length() + 1;
	_hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(_hMem), content.c_str(), len);
	GlobalUnlock(_hMem);

	SetClipboardData(CF_TEXT, _hMem);
}

std::string ClipBoard::ClipBoardRead()
{
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (!hData)
	{
		return "";
	}
	char* pszText = static_cast<char*>(GlobalLock(hData));

	std::string result(pszText);
	GlobalUnlock(hData);

	return result;
}

void ClipBoard::clearClipBoard()
{
	EmptyClipboard();
}

