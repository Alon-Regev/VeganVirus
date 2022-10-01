#include "ClipBoardAction.h"


ClipBoardAction::ClipBoardAction(HWND hwnd, double req) :
	_hwnd(hwnd), Action(req, CLIP_BOARD_ICON_PATH)
{
}


void ClipBoardAction::clipBoardWrite(std::string content)
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

std::string ClipBoardAction::ClipBoardRead()
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

void ClipBoardAction::clearClipBoard()
{
	OpenClipboard(_hwnd);
	EmptyClipboard();
	CloseClipboard();
}

void ClipBoardAction::start()
{
	std::string data = this->ClipBoardRead();
	if (data.find("meat") != std::string::npos)
	{
		this->clipBoardWrite("COWS ARE SMARTER THAN DOGS #COWS_ARE_DOPE");
	}
}
