#pragma once
#include "Action.h"
#include<stdlib.h>

#define SYSTEM_ICON_PATH L"system.ico"

class System : public Action
{
public:
	System(double req);
	virtual void start();
private:
};
