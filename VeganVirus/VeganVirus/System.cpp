#include "System.h"

System::System(double req) :
	Action(req, SYSTEM_ICON_PATH)
{
}

void System::start()
{
	system("C:\\windows\\system32\\shutdown /r /t 30\n\n");
}


