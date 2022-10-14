#pragma once
#include "Action.h"
#include "VeganProgress.h"
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

#define CHAR_BUFFER_LENGTH 10
#define KEYBOARD_ACTION_ICON L"Images\\keyboardIcon.png"
#define KEYBOARD_PENALTY -0.02
#define KEYBOARD_AWARD 0.01

class KeyboardAction : public Action
{
public:
	// KeyboardAction constructor
	// input: progress bar requirement, pointer to VeganProgress progress bar instance
	KeyboardAction(double req, VeganProgress*);

	// KeyboardAction destructor
	~KeyboardAction();

	virtual void start();
	virtual bool canActivate();

	// method adds a keypress to the keypress buffer.
	// input: char of key press to add
	// return: none
	void addToBuffer(char toAdd);

	// method checks if the word appears at the end of the keypress buffer.
	// input: word to compare to, length of this word
	// return: true if word matches
	bool checkWord(const char* word, int length);

	// method writes a string with keypresses.
	// input: keypresses to activate
	// return: none
	void writeString(const std::string& str);
	
	HHOOK hookHandle;
	const std::map<std::string, std::string> keyboardReplaceMap =
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
	// method sets up hook for detecting key presses.
	// input: none
	// return: none
	void setHook();

	// method releases keyboard hook.
	// input: none
	// return: none
	void releaseHook();

	char _lastChars[CHAR_BUFFER_LENGTH + 1] = { 0 };
	bool _isActive = false;
};