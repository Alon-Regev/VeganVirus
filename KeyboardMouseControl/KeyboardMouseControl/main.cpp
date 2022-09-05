#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <map>

#define CHAR_BUFFER_LENGTH 10

std::map<std::string, std::string> keyboardReplaceMap =
{
	{"MEAT", "VEGETABLE"},
	{"MILK", "SOY MILK"},
	{"EGG", "BABY CHICKEN"}
};

HHOOK hookHandle;

// buffer of last keyboard inputs
char lastChars[CHAR_BUFFER_LENGTH + 1] = { 0 };

// function adds char to buffer
void addToBuffer(char toAdd)
{
	// copy buffer one place back
	memmove(lastChars, lastChars + 1, CHAR_BUFFER_LENGTH - 1);
	// add to end
	lastChars[CHAR_BUFFER_LENGTH - 1] = toAdd;
}

// checks if word is at the end of the buffer
bool checkWord(const char* word, int length)
{
	return !strcmp(lastChars + CHAR_BUFFER_LENGTH - length, word);
}

// function writes a string with the keyboard
void writeString(std::string str)
{
	std::vector<INPUT> inputs;
	for (int i = 0; i < str.size(); i++)
	{
		// init key down struct
		INPUT inputStruct = { 0 };
		inputStruct.type = INPUT_KEYBOARD;
		KEYBDINPUT kbdInput = { 0 };
		kbdInput.wVk = str[i];
		inputStruct.ki = kbdInput;

		// init key up struct
		INPUT inputUpStruct = inputStruct;
		inputUpStruct.ki.dwFlags = KEYEVENTF_KEYUP;

		inputs.push_back(inputStruct);
		inputs.push_back(inputUpStruct);
	}
	// send keyboard inputs
	SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

// callback for a keypress (only checks key down)
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || wParam != WM_KEYUP)
		return CallNextHookEx(hookHandle, nCode, wParam, lParam);
	// key down event

	// get data struct
	KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

	// main characters (letters and digits) are the same as ascii (uppercase)
	// so stored as ascii
	addToBuffer(kbdStruct.vkCode);

	// check if typed word
	if (checkWord("LIRON", 5))
	{
		std::string toWrite(5, '\b');
		toWrite += "ALON";
		writeString(toWrite);
	}

	return CallNextHookEx(hookHandle, nCode, wParam, lParam);
}

// function sets up keyboard hook
void SetHook()
{
	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
	if (!hookHandle)
		MessageBoxA(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
}

// function releases windows hook
void ReleaseHook()
{
	UnhookWindowsHookEx(hookHandle);
}

void main()
{
	SetHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0));
	ReleaseHook();
}