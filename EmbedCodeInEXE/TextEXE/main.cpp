#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

int main()
{
	OFSTRUCT temp = { 0 };
	sizeof(OFSTRUCT);
	HFILE infile = OpenFile("a.txt", &temp, OF_READ);
	SetFilePointer((HANDLE)infile, 5, NULL, FILE_BEGIN);
	
	HFILE outfile = OpenFile("c.txt", &temp, OF_WRITE | OF_CREATE);
	
	char a = 0;
	int check = 1;
	while(check)
	{
		int b = ReadFile((HANDLE)infile, &a, 1, (LPDWORD)&check, NULL);
		b = WriteFile((HANDLE)outfile, &a, check, NULL, NULL);
	}
	
	CloseHandle((HANDLE)infile);
	CloseHandle((HANDLE)outfile);
}