#include "ClipBoard.h"


ClipBoard::ClipBoard(HWND hwnd) : _hwnd(hwnd)
{
	//OpenClipboard(hwnd);
}

ClipBoard::~ClipBoard()
{
	//CloseClipboard();
}

void ClipBoard::clipBoardWrite(std::string content)
{
	OpenClipboard(_hwnd);
	EmptyClipboard();
	const size_t len = content.length() + 1;
	_hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(_hMem), content.c_str(), len);
	GlobalUnlock(_hMem);

	SetClipboardData(CF_TEXT, _hMem);
	CloseClipboard();
}

std::string ClipBoard::ClipBoardRead()
{

	OpenClipboard(_hwnd);
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (!hData)
	{
		return "";
	}
	char* pszText = static_cast<char*>(GlobalLock(hData));

	std::string result(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return result;
	

}

void ClipBoard::clearClipBoard()
{
	OpenClipboard(_hwnd);
	EmptyClipboard();
	CloseClipboard();
}

