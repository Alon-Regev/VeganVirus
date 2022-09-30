#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "FruitThrow.h"
#include <list>

#define FRUIT_THROW_ICON L"vegetables.png"
#define FRUIT_THROW_REQ 0.9975

#define MIN_FRUIT_TIMER 0.4
#define MAX_FRUIT_TIMER 1.25
#define FRUIT_COUNT 12

#define COMPUTATION_SUB_FRAME_INTERVAL 0.0075

#define CURSOR_FRICTION -45
// accelaration from friction (px/s^2)

class FruitThrowAction :
    public Action
{
public:
    FruitThrowAction(Draw* draw, MouseManager& mouseManager);
    ~FruitThrowAction();

    virtual void start();

    virtual void update(double dt);

private:
    void subFrameUpdate(double dt, double x1, double y1);

    Draw* _draw;
    MouseManager& _mouseManager;
    double _timer = 0;
    double _fruitLeft = 0;

    double _vx = 0, _vy = 0;
    double _px = 0, _py = 0;
    bool _mouseDisabled = false;

    std::list<FruitThrow*> _currentFruits;
};

