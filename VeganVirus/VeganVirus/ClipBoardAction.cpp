#include "ClipBoardAction.h"


ClipBoardAction::ClipBoardAction(double req) : Action(req, CLIP_BOARD_ICON_PATH)
{
}

void ClipBoardAction::start()
{
	this->activeFlag = true;
}

void ClipBoardAction::update(double dt)
{
	if (!this->activeFlag)
	{
		return;
	}
	std::string data = this->ClipBoardRead();
	if (data.find("meat") != std::string::npos)
	{
		this->clipBoardWrite("COWS ARE SMARTER THAN DOGS TEL_AVIV");
	}
	Sleep(100);
}

void ClipBoardAction::clipBoardWrite(std::string content)
{
	OpenClipboard(NULL);
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
	OpenClipboard(NULL);
	HANDLE hData = GetClipboardData(CF_TEXT);
	CloseClipboard();

	if (!hData)
	{
		return "";
	}
	char* pszText = static_cast<char*>(GlobalLock(hData));

	std::string result(pszText);
	GlobalUnlock(hData);
	
	return result;
}



