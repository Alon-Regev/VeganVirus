#pragma once
#include "Action.h"
#include <stdio.h> 
#include <string.h>
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>

#define EXCEL_ICON_PATH L"Images\\excel.ico"
#define EXCEL_FILE_EXTENSION ".xlsx"
#define TITLE_LEN 100
#define EXCEL_DOCUMENT_PATH "liron.xlsx"

typedef struct
{
	int count;
	const char* searchTerm;
} WindowCountCallbackParams;

class ExcelAction : public Action
{
public:
	ExcelAction(double req);
	
	virtual void start();
	virtual void update(double dt);

private:
	//brief Check if a process is running
	int processCount(const char* searchTerm);
	//opens file
	void openFile(int num);
	//create file
	void createFile(int num);
	//delete file
	void deleteFile(int num, std::set<int> arr);
	//deleteing all files inside vecter
	void deleteAllExcelFile(std::set<int> arr);
	//count the number of excel windows
	int excelWindowCount();

	std::set<int> _vectorExcelFiles;
	int _preProcessCount;
	int num_file = 1;
};



