#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

void setFileEntryPoint(const char* szFilename, long newEntryPoint);

int main()
{
    int x = 0;
    std::string exePath;
    printf("Enter executable name: ");
    std::getline(std::cin, exePath);
    printf("Enter new entry point: ");
    scanf("%x", &x);
    setFileEntryPoint(exePath.c_str(), x);
}

void setFileEntryPoint(const char* szFilename, long newEntryPoint)
{
    IMAGE_DOS_HEADER* pHead;
    pHead = new IMAGE_DOS_HEADER;
    FILE* file = fopen(szFilename, "r+b");
    if (!file)
    {
        printf("Can't open file :(\n");
        return;
    }
    fread(pHead, sizeof(IMAGE_DOS_HEADER), 1, file);
    if (pHead->e_magic != IMAGE_DOS_SIGNATURE)
    {
        printf("Invalid magic :(\n");
        return;
    }
    long peHeader = pHead->e_lfanew;
    long peOptHeader = peHeader + 4 + sizeof(IMAGE_FILE_HEADER);
    fseek(file, peOptHeader, SEEK_SET);
    IMAGE_OPTIONAL_HEADER* pOpt;
    pOpt = new IMAGE_OPTIONAL_HEADER;
    fread(pOpt, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);
    printf("Entry point: %x\n", pOpt->AddressOfEntryPoint);
    pOpt->AddressOfEntryPoint = newEntryPoint;
    fseek(file, peOptHeader, SEEK_SET);
    fwrite(pOpt, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);
    fclose(file);
    delete pOpt;
    delete pHead;
}