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



class TaskManagerHide
{
public:
	/*
	the function start the actoin
	*/
	void start();
	/*
	the function find all the process with `name`
	*/
	void findProcess(const char* name);
private:
	/*
	the function inject the dll to the process
	*/
	void injectProcess(HANDLE);
	
	HANDLE _thread;
};
