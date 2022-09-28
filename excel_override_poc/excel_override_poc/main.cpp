#include <windows.h>
#include <stdio.h> 
#include <string.h>
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>
#define EXCEL_FILE_EXTENSION ".xlsx"

typedef struct
{
    int count;
    const char* searchTerm;
} WindowCountCallbackParams;

BOOL CALLBACK windowCountCallback(
    HWND hwnd,
    LPARAM lParam
)
{
    WindowCountCallbackParams& params = *(WindowCountCallbackParams*)lParam;

    char title[100] = { 0 };
    GetWindowTextA(hwnd, title, 100);
    char* ret;

    // check if title contains params.searchTerm
    ret = strstr(title, params.searchTerm);
    if (ret != NULL) {
        params.count++;
    }
    return TRUE;
}
/*!
\brief Check if a process is running
\param [in] processName Name of process to check if is running
\returns \c True if the process is running, or \c False if the process is not running
*/
int processCount(const char* searchTerm)
{
    WindowCountCallbackParams params = { 0, searchTerm };
    EnumWindows(windowCountCallback, (LPARAM)&params);
    return params.count;

}
/*!
    opens file 
    param int num of the file
    return None
*/
void openFile(int num)
{
    std::string fileToOpen = std::to_string(num) + EXCEL_FILE_EXTENSION;
    system(fileToOpen.c_str());

}
/*!
    create file 
    param int num of the file
    return None
*/
void createFile(int num)
{
    std::string fileToCreat = std::to_string(num) + EXCEL_FILE_EXTENSION;
    bool ret = CopyFileA("liron.xlsx", fileToCreat.c_str(), TRUE);
    if (!ret)
    {
        printf("error %d\n", GetLastError());
    }
}
/*!
    delate file 
    param int num of the file and set 
    return None
*/
void delatFile(int num, std::set<int> arr)
{
    std::string fileToRemov = std::to_string(num) + EXCEL_FILE_EXTENSION;
    int result = DeleteFileA(fileToRemov.c_str());
    if (result != 0)
    {
        arr.erase(num);
    }  
}
/*!
    delateing all files inside vecter
    param set 
    return None
*/
void delateAllExcelFile(std::set<int> arr)
{
    for (auto& i : arr) {
        delatFile(i, arr);
    }
}
/*!
    count the number of excel windows 
    param None
    return int
*/
int excelWindowCount()
{
    int excelWindowCount = processCount("Excel") - 1;
    if (excelWindowCount == -1)
    {
        excelWindowCount = 0;
    }
    return  excelWindowCount;
}
int main() 
{
    std::set<int> vectorExcelFiles;
    int preProcessCount = excelWindowCount();
    int num_file = 1;
    while (true)
    {
        int currPocessCount = excelWindowCount();;
        if (currPocessCount > preProcessCount)
        {
            createFile(num_file);
            openFile(num_file);
            vectorExcelFiles.insert(num_file);
            num_file += 1;
            currPocessCount++;
            Sleep(1000);
        }
        else if (currPocessCount < preProcessCount)
        {
            delateAllExcelFile(vectorExcelFiles);
        }
        preProcessCount = currPocessCount;
        printf("%d\n", currPocessCount);
    }
	return 0;
  
}