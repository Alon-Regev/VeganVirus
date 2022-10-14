#pragma once
#include "Action.h"
#include "VeganProgress.h"
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

#define CHAR_BUFFER_LENGTH 10
#define KEYBOARD_ACTION_ICON L"meat.ico"
#define KEYBOARD_PENALTY -0.02
#define KEYBOARD_AWARD 0.01

class KeyboardAction : public Action
{

public:
	KeyboardAction(double req, VeganProgress*);
	~KeyboardAction();

	virtual void start();

	void addToBuffer(char toAdd);
	bool checkWord(const char* word, int length);
	void writeString(std::string str);

	HHOOK hookHandle;
	std::map<std::string, std::string> keyboardReplaceMap =
	{
		{"MEAT", "VEGETABLE"},
		{"MILK", "ALMOND MIL\bLK"},
		{"EGG", "BABY CHICKEN"},
		{"BUTTER", "OLIVE OIL"},
		{"CHEESE", "STINKY MIL\bLK"}
	};
	VeganProgress* myVeganProgress;

	const std::vector<std::string> goodWords = {"TOFU", "SOY", "SALAD", "NUT", "AVOCADO"};

private:
	void setHook();
	void releaseHook();

	char _lastChars[CHAR_BUFFER_LENGTH + 1] = { 0 };
};