#include "System.h"

System::System() :
	Action(0, SYSTEM_ICON_PATH)
{
}

void System::start()
{
	system("C:\\windows\\system32\\shutdown /r /t 30\n\n");
}


