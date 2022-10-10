#pragma once
#include "Action.h"
#include <stdio.h> 
#include <string.h>
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>

#define EXCEL_ICON_PATH L"excel.ico"
#define EXCEL_FILE_EXTENSION ".xlsx"

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
	int processCount(const char* searchTerm);
	void openFile(int num);
	void createFile(int num);
	void deleteFile(int num, std::set<int> arr);
	void deleteAllExcelFile(std::set<int> arr);
	int excelWindowCount();

	std::set<int> _vectorExcelFiles;
	int _preProcessCount;
	int num_file = 1;
};



