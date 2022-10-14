#pragma once
#include "Action.h"
#include "VeganProgress.h"
#include <string>
#include <windows.h> 
#include <algorithm>
#include <map>

#define CLIP_BOARD_ICON_PATH L"clipBoard.ico"
#define CLIP_BOARD_PENALTY -0.03
#define ALLOWED_WORD "animal corpse" 

class ClipBoardAction : public Action
{
public:
	/*
	class constructor
	param req: stage of use
	return: none
	*/
	ClipBoardAction(double req, VeganProgress*);

	virtual void start();
	virtual void update(double dt);


private:
	
	std::map<std::string, std::string> _keyboardReplaceMap =
	{
		{"meat", "vegetable"},
		{"milk", "almond mil\blk"},
		{"egg", "baby chicken"},
		{"butter", "olive oil"},
		{"cheese", "stinky mil\blk"}
	};

	/*
	the function write `content` to the clipboard
	param content: the data to pass
	return: none
	*/
	void clipBoardWrite(std::string content);
	/*
	the function read data from the cilpboard
	param: none
	return: the data that was read
	*/
	std::string ClipBoardRead();

	HGLOBAL _hMem;
	VeganProgress* _veganProgress;
};
