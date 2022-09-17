#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

int main()
{
	printf("hello: 0x%X\n", CreateFileA);
	printf("hello: 0x%X\n", GetLastError);
	GetLastError();
	printf("hi\n");
}