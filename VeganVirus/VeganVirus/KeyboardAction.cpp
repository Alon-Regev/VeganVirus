#include "KeyboardAction.h"

#define KEYBOARD_PENALTY -0.04
#define KEYBOARD_AWARD 0.03

KeyboardAction* keyboardActionInstance = nullptr;

KeyboardAction::KeyboardAction(double req, VeganProgress* veganProgress)
	: Action(req, KEYBOARD_ACTION_ICON), myVeganProgress(veganProgress)
{
	keyboardActionInstance = this;
}

// callback for a keypress (only checks key down)
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || wParam != WM_KEYUP)
		return CallNextHookEx(keyboardActionInstance->hookHandle, nCode, wParam, lParam);
	// key down event

	// get data struct
	KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

	// main characters (letters and digits) are the same as ascii (uppercase)
	// so stored as ascii
	keyboardActionInstance->addToBuffer(kbdStruct.vkCode);

	// check if typed word
	for (auto pair : keyboardActionInstance->keyboardReplaceMap)
	{
		//check if in list of bad words
		if (keyboardActionInstance->checkWord(pair.first.c_str(), pair.first.size()))
		{
			//decrease bar meat 
			keyboardActionInstance->myVeganProgress->addProgress(-0.05);
			//replace the word
			std::string toWrite(pair.first.size(), '\b');
			toWrite += pair.second;
			keyboardActionInstance->writeString(toWrite);
		}
		//check if in list of good words
		if (keyboardActionInstance->checkWord(pair.second.c_str(), pair.second.size()))
		{
			keyboardActionInstance->myVeganProgress->addProgress(0.01);
		}
	}
	
	return CallNextHookEx(keyboardActionInstance->hookHandle, nCode, wParam, lParam);
}

void KeyboardAction::setHook()
{
	hookHandle = SetWindowsHookExW(WH_KEYBOARD_LL, HookCallback, NULL, 0);
	if (!hookHandle)
		MessageBoxA(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
}

void KeyboardAction::addToBuffer(char toAdd)
{
	// copy buffer one place back
	memmove(_lastChars, _lastChars + 1, CHAR_BUFFER_LENGTH - 1);
	// add to end
	_lastChars[CHAR_BUFFER_LENGTH - 1] = toAdd;
}

bool KeyboardAction::checkWord(const char* word, int length)
{
	return !strcmp(_lastChars + CHAR_BUFFER_LENGTH - length, word);
}

void KeyboardAction::writeString(std::string str)
{
	std::vector<INPUT> inputs;
	for (int i = 0; i < str.size(); i++)
	{
		// init key down struct
		INPUT inputStruct = { 0 };
		inputStruct.type = INPUT_KEYBOARD;
		inputStruct.ki.wVk = str[i];

		// init key up struct
		INPUT inputUpStruct = inputStruct;
		inputUpStruct.ki.dwFlags = KEYEVENTF_KEYUP;

		inputs.push_back(inputStruct);
		inputs.push_back(inputUpStruct);
	}
	// send keyboard inputs
	SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

void KeyboardAction::releaseHook()
{
	UnhookWindowsHookEx(hookHandle);
}

KeyboardAction::~KeyboardAction()
{
	this->releaseHook();
}

void KeyboardAction::start()
{
	this->setHook();
}
