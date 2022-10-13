#pragma once
#include "Action.h"
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

#define CHAR_BUFFER_LENGTH 10
#define KEYBOARD_ACTION_ICON L"meat.ico"

class KeyboardAction : public Action
{

public:
	KeyboardAction(double req);
	~KeyboardAction();

	virtual void start();
	virtual void update(double dt);

	void addToBuffer(char toAdd);
	bool checkWord(const char* word, int length);
	void writeString(std::string str);

	HHOOK hookHandle;
	std::map<std::string, std::string> keyboardReplaceMap =
	{
		{"MEAT", "VEGETABLE"},
		{"MILK", "SOY MIL\bLK"},
		{"EGG", "BABY CHICKEN"}
	};
	
private:
	void setHook();
	void releaseHook();

	char _lastChars[CHAR_BUFFER_LENGTH + 1] = { 0 };
};
