#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "FruitThrow.h"
#include <list>

#define FRUIT_THROW_ICON L"vegetables.png"
#define FRUIT_THROW_REQ 0.9975

class FruitThrowAction :
    public Action
{
public:
    FruitThrowAction(Draw* draw, MouseManager& mouseManager);
    ~FruitThrowAction();

    virtual void start();

    virtual void update(double dt);

private:
    Draw* _draw;
    MouseManager& _mouseManager;

    std::list<FruitThrow*> _currentFruits;
};

