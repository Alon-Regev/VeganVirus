#pragma once
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <comdef.h>

#define DLL_NAME L"HideProcessHookDLL.dll"
#define INJECT_INTO "Taskmgr.exe"


class TaskManager
{
public:
	void start();

private:
	void injectProcess(HANDLE);
	void findProcess(const char* name);
};


