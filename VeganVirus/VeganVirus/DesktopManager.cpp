#include "DesktopManager.h"

DesktopManager::DesktopManager()
{
    this->reset();
}

DesktopManager::~DesktopManager()
{
    VirtualFreeEx(this->_hListViewProcess, this->_pPointData, 0, MEM_RELEASE);
    CloseHandle(this->_hListViewProcess);
}

POINT DesktopManager::getIconPosition(int i)
{
    bool ret = ListView_GetItemPosition(this->_hDesktopListView, i, this->_pPointData);
    POINT p = { 0 };
    ret |= ReadProcessMemory(this->_hListViewProcess, this->_pPointData, &p, sizeof(POINT), nullptr);
    if (!ret)
        ;// throw std::exception("Can't read position data");
    return p;
}

bool DesktopManager::setIconPosition(int i, POINT p)
{
    bool ret = ListView_SetItemPosition(this->_hDesktopListView, i, p.x, p.y);
    return ret;
}

void DesktopManager::moveIcon(int i, POINT p)
{
    POINT currPosition = getIconPosition(i);
    currPosition.x += p.x;
    currPosition.y += p.y;
    setIconPosition(i, currPosition);
}

int DesktopManager::iconCount()
{
    return ListView_GetItemCount(this->_hDesktopListView);
}

void DesktopManager::reset()
{
    // get handle to list view
    HWND hShellWnd = GetShellWindow();
    HWND hDefView = FindWindowEx(hShellWnd, NULL, L"SHELLDLL_DefView", NULL);
    this->_hDesktopListView = FindWindowEx(hDefView, NULL, L"SysListView32", NULL);
    if (!this->_hDesktopListView)
    {
        // first method failed. try a different one
        HWND destop = GetDesktopWindow();
        HWND hWorkerW = NULL;
        HWND hShellViewWin = NULL;
        do
        {
            hWorkerW = FindWindowEx(destop, hWorkerW, L"WorkerW", NULL);
            hShellViewWin = FindWindowEx(hWorkerW, 0, L"SHELLDLL_DefView", 0);
        } while (hShellViewWin == NULL && hWorkerW != NULL);
        this->_hDesktopListView = FindWindowEx(hShellViewWin, NULL, L"SysListView32", NULL);
        if (!this->_hDesktopListView)
            throw std::exception("Can't get desktop list view");
    }

    // init list view process handle (for getting information from it)
    DWORD desktop_proc_id = 0;
    GetWindowThreadProcessId(this->_hDesktopListView, &desktop_proc_id);
    this->_hListViewProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ, FALSE, desktop_proc_id);

    // allocate memory for getting position data
    this->_pPointData = (LPPOINT)VirtualAllocEx(this->_hListViewProcess, NULL, sizeof(POINT), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!this->_hListViewProcess || !this->_pPointData)
        throw std::exception("Can't access desktop list view process");
}
