#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>

void cuteFunction()
{
	int x = 0;
	scanf("%d", &x);
	printf("hi %d", x);
	_getch();
}

int main()
{
	cuteFunction();
	printf("\nmain: %x\nuteFunction: %x\n", main, cuteFunction);
}