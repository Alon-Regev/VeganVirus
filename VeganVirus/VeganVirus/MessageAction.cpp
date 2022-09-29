#include "MessageAction.h"
#include <Windows.h>

MessageAction::MessageAction(double req, const char* message) :
	Action(req, EXAMPLE_ICON_PATH), _message(message)
{
}

void MessageAction::start()
{
	MessageBoxA(NULL, this->_message, "Hello friend", 0);
}
