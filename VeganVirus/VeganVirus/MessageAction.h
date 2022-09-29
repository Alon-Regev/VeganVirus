#pragma once
#include "Action.h"

#define MESSAGE_ICON_PATH L"message.ico"

class MessageAction :
    public Action
{
public:
    // MessageAction Constructor
    // input: requirement to start the action
    MessageAction(double req, const char* message);

    virtual void start();

private:
    const char* _message;
};

