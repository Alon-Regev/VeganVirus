#include "ExcelAction.h"



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
ExcelAction::ExcelAction(double req) : Action(req, EXCEL_ICON_PATH)
{
}
void ExcelAction::start()
{
    _preProcessCount = excelWindowCount();
    this->activeFlag = true;
}
void ExcelAction::update(double dt)
{
    if (!this->activeFlag)
    {
        return;
    }
    int currPocessCount = excelWindowCount();;
    if (currPocessCount > _preProcessCount)
    {
        createFile(num_file);
        openFile(num_file);
        _vectorExcelFiles.insert(num_file);
        num_file += 1;
        currPocessCount++;
        Sleep(1000);
    }
    else if (currPocessCount < _preProcessCount)
    {
        deleteAllExcelFile(_vectorExcelFiles);
    }
    _preProcessCount = currPocessCount;
}
/*!
\brief Check if a process is running
\param [in] processName Name of process to check if is running
\returns \c True if the process is running, or \c False if the process is not running
*/
int ExcelAction::processCount(const char* searchTerm)
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
void ExcelAction::openFile(int num)
{
    std::string fileToOpen = std::to_string(num) + EXCEL_FILE_EXTENSION;
    system(fileToOpen.c_str());

}
/*!
    create file
    param int num of the file
    return None
*/
void ExcelAction::createFile(int num)
{
    std::string fileToCreat = std::to_string(num) + EXCEL_FILE_EXTENSION;
    bool ret = CopyFileA("liron.xlsx", fileToCreat.c_str(), TRUE);
    if (!ret)
    {
        printf("error %d\n", GetLastError());
    }
}
/*!
    delete file
    param int num of the file and set
    return None
*/
void ExcelAction::deleteFile(int num, std::set<int> arr)
{
    std::string fileToRemov = std::to_string(num) + EXCEL_FILE_EXTENSION;
    int result = DeleteFileA(fileToRemov.c_str());
    if (result != 0)
    {
        arr.erase(num);
    }
}
/*!
    deleteing all files inside vecter
    param set
    return None
*/
void ExcelAction::deleteAllExcelFile(std::set<int> arr)
{
    for (auto& i : arr) {
        deleteFile(i, arr);
    }
}
/*!
    count the number of excel windows
    param None
    return int
*/
int ExcelAction::excelWindowCount()
{
    int excelWindowCount = processCount("Excel") - 1;
    if (excelWindowCount == -1)
    {
        excelWindowCount = 0;
    }
    return  excelWindowCount;
}