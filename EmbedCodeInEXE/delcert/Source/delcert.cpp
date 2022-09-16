// delcert.cpp
//
// An app to make hiterto unsignable file(s) signable again.
//
// 8/10/2006 - Drew
// 7/19/2008 - deepred
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <imagehlp.h>
#include <malloc.h>
#include <tchar.h>

#define REGULARFILE(x) ((x) & ~(FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_ENCRYPTED | FILE_ATTRIBUTE_VIRTUAL))

DWORD StripAuthenticode(LPTSTR);

int wmain(DWORD argc, LPTSTR argv[]) {
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	DWORD dwResult = ERROR_SUCCESS;
	TCHAR lpBuffer[4096] = {0};
	LPTSTR lpPart = NULL;
	TCHAR pszFileDirectory[4096] = {0};
	TCHAR pszFileFullName[8192] = {0};

	_tprintf(_TEXT("\n"));
	if (2 != argc || 0 == _tcscmp(_TEXT("-?"), argv[1]) || 0 == _tcscmp(_TEXT("/?"), argv[1])) {
		_tprintf(_TEXT("%s takes one parameter - a file name to strip of its embedded Authenticode signature.\n\n"), argv[0]);
		return 0;
	}

	if (0 == GetFullPathName(argv[1], 4096, lpBuffer, &lpPart)) {
		_tprintf(_TEXT("Failed to GetFullPathName."));
		return 0;
	}
	if (_tcsstr(lpBuffer, lpPart)) {
		_tcsncpy(pszFileDirectory, lpBuffer, _tcslen(lpBuffer) - _tcslen(lpPart));
	}

	_tprintf(_TEXT("Target file(s): %s\n\n"), argv[1]);
	hFindFile = FindFirstFile(argv[1], &FindFileData);
	if (INVALID_HANDLE_VALUE == hFindFile) {
		dwResult = GetLastError();
		_tprintf(_TEXT("Invalid File Handle. GLE = 0x%08x\n"), dwResult);
	} else {
		if (REGULARFILE(FindFileData.dwFileAttributes)) {
			if (-1 == _stprintf_s(pszFileFullName, 8192, _TEXT("%s%s"), pszFileDirectory, FindFileData.cFileName)) {
				dwResult = GetLastError();
				_tprintf(_TEXT("Failed to copy pszFileName to string of chars. GLE == x0%08x\n"), dwResult);
				goto exit;
			}
			dwResult = StripAuthenticode(pszFileFullName);
		}
		while (TRUE) {
			if (FindNextFile(hFindFile, &FindFileData)) {
				if (REGULARFILE(FindFileData.dwFileAttributes)) {
					if (-1 == _stprintf_s(pszFileFullName, 8192, _TEXT("%s%s"), pszFileDirectory, FindFileData.cFileName)) {
						dwResult = GetLastError();
						_tprintf(_TEXT("Failed to copy pszFileName to string of chars. GLE == x0%08x\n"), dwResult);
						goto exit;
					}
					dwResult = StripAuthenticode(pszFileFullName);
				}
			} else {
				dwResult = GetLastError();
				break;
			}
		}
	}

exit:
	if (INVALID_HANDLE_VALUE != hFindFile) FindClose(hFindFile);
	return dwResult;
}

DWORD StripAuthenticode(LPTSTR pszFileName) {
	HANDLE hFile = INVALID_HANDLE_VALUE;
	LOADED_IMAGE image;
	DWORD dwResult = ERROR_SUCCESS;
	LPSTR lpszImageName = NULL;
	size_t cchImageName = 0;

	_tprintf(_TEXT("Stripping file: %s.\n"), pszFileName);
	hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		dwResult = GetLastError();
		_tprintf(_TEXT("CreateFile failed with error 0x%08x\n"), dwResult);
		goto cleanupAndExit;
	}

	if(ImageRemoveCertificate(hFile, 0)) {
		goto cleanupAndExit;
	} else {
		dwResult = GetLastError();
		_tprintf(_TEXT("ImageRemoveCertificate failed with error 0x%08x\n"), dwResult);
		if(ERROR_INVALID_PARAMETER != dwResult) {
			goto cleanupAndExit;
		} else {
			_tprintf(_TEXT("This happens when there's a listing in IMAGE_DIRECTORY_SECURITY\nin the PE's header, but the actual Authenticode signature has been stripped.\nLet's fix that ...\n"));
			dwResult = ERROR_SUCCESS;
		}
	}

	if(CloseHandle(hFile)) hFile = INVALID_HANDLE_VALUE;
	// This is somewhat sloppy, but if we're here we've almost certainly found a PE with an
	// IMAGE_DIRECTORY_SECURITY that has nonzero SizeOfRawData and/or PointerToRawData,
	// but the actual signature (that raw data) has been removed.
	//
	// What causes this? IIRC, strong name signing something that's already been Authenticode-signed.
	//
	// The workaround is to crack open the PE and write zeros into the directory entry so that everything
	// that eventually calls through the Image*Certificate* APIs won't choke.

	cchImageName = _tcslen(pszFileName) + 1;
	lpszImageName = (LPSTR)malloc(cchImageName); // Yeah - so I'm all old-school mallocy!
	if(!lpszImageName) {
		dwResult = GetLastError();
		_tprintf(_TEXT("Malloc failed. GLE == 0x%08x\n"), dwResult);
		goto cleanupAndExit;
	}

	if (-1 == sprintf_s(lpszImageName, cchImageName, "%S", pszFileName)) {
		dwResult = GetLastError();
		_tprintf(_TEXT("Failed to copy pszFileName to string of chars. GLE == x0%08x\n"), dwResult);
		goto cleanupAndExit;
	}

	if(!MapAndLoad(lpszImageName, NULL, &image, FALSE, FALSE)) {
		dwResult = GetLastError();
		_tprintf(_TEXT("MapAndLoad failed. GLE == 0x%08x"), dwResult);
		goto cleanupAndExit;
	}

	_tprintf(_TEXT("certificates->Size == 0x%08x\n"), image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
	_tprintf(_TEXT("certificates->VA == 0x%08x\n"), image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);

	if (image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size ||
		image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress) {
		_tprintf(_TEXT("Setting both fields to zero ...\n"));
		image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size = 0;
		image.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress = 0;
	} else {
		_tprintf(_TEXT("Fields are set to zero already. Skipping ...\n"));
	}

	if(!UnMapAndLoad(&image)) {
		dwResult = GetLastError();
		_tprintf(_TEXT("Failed to UnMapAndLoad. GLE == 0x%08x\n"), dwResult);
		goto cleanupAndExit;
	}

cleanupAndExit:
	if (INVALID_HANDLE_VALUE != hFile) CloseHandle(hFile);
	if (lpszImageName) free(lpszImageName);
	if (ERROR_SUCCESS == dwResult) _tprintf(_TEXT("Succeeded.\n"));
	_tprintf(_TEXT("\n"));
	return dwResult;
}