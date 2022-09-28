#pragma once
#include "Action.h"

#define EXAMPLE_ICON_PATH "VeganVirus.ico"

class ExampleAction :
    public Action
{
public:
    ExampleAction();

    virtual void start();
};

