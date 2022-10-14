#pragma once
#include "Action.h"
#include<stdlib.h>

#define SYSTEM_ICON_PATH L"Images\\system.ico"

class SystemAction : public Action
{
public:
	SystemAction(double req);
	virtual void start();
private:
};
