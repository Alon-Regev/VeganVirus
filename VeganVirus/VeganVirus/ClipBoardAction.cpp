#include "ClipBoardAction.h"


ClipBoardAction::ClipBoardAction(double req, VeganProgress* veganProgress) 
	: Action(req, CLIP_BOARD_ICON_PATH), _veganProgress(veganProgress)
{
}

void ClipBoardAction::start()
{
	this->activeFlag = true;
}

void ClipBoardAction::update(double dt)
{
	std::string forbiddenWords[6] = { "meat", "beef", "chicken", "pigs", "bacon", "lamb" };
	if (!this->activeFlag)
	{
		return;
	}
	std::string data = this->ClipBoardRead();

	//convert to lower case string
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
	});
	
	for (int i = 0; i < 6; i++)
	{
		size_t location = data.find(forbiddenWords[i]);
		if (location != std::string::npos)
		{
			data.replace(location, std::size(forbiddenWords[i]), ALLOWED_WORD);
			this->clipBoardWrite(data);
			this->_veganProgress->addProgress(CLIP_BOARD_PENALTY);
		}
	}
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



