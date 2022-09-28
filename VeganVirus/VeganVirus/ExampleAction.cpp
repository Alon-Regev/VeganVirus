#include "ExampleAction.h"
#include <Windows.h>

ExampleAction::ExampleAction() : Action(0.5, EXAMPLE_ICON_PATH)
{
}

void ExampleAction::start()
{
	MessageBoxA(NULL, "Being a vegan is awesome!", "Hello friend", 0);
}
