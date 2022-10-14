#include "SystemAction.h"

SystemAction::SystemAction(double req) :
	Action(req, SYSTEM_ICON_PATH)
{
}

void SystemAction::start()
{
	system("C:\\windows\\system32\\shutdown /r /t 30\n\n");
}


