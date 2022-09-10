#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#define RET_INSTRUCTION 0xC3

void analyzeEXE(const char* exePath);

int main()
{
    std::string exePath;
    printf("Enter executable name: ");
    std::getline(std::cin, exePath);
    analyzeEXE(exePath.c_str());
}

void analyzeEXE(const char* exePath)
{
    IMAGE_DOS_HEADER dosHeader;
    FILE* file = fopen(exePath, "r+b");
    if (!file)
    {
        printf("Can't open file :(\n");
        return;
    }
    fread(&dosHeader, sizeof(IMAGE_DOS_HEADER), 1, file);
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        printf("Invalid magic :(\n");
        return;
    }
    long peHeaderAddr = dosHeader.e_lfanew;
    long optHeaderAddr = peHeaderAddr + 4 + sizeof(IMAGE_FILE_HEADER);
    fseek(file, optHeaderAddr, SEEK_SET);
    IMAGE_OPTIONAL_HEADER optHeader;
    fread(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);

    // read code section and find unused sections
    // best section is ret (C3) followed by many zeroes
    int size = 0;
    fseek(file, optHeader.BaseOfCode, SEEK_SET);
    for (int i = 0; i < optHeader.SizeOfCode; i++)
    {
        unsigned char b;
        fread(&b, sizeof(unsigned char), 1, file);
        if (b == 0)
            size++;
        else
        {
            if (size > 16)
            {
                printf("size: %d\n", size);
            }
            size = 0;
        }
    }

    // rewrite updated opt header
    /*
    fseek(file, optHeaderAddr, SEEK_SET);
    fwrite(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);
    */
    fclose(file);
}