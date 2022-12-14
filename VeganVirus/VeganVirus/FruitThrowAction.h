#pragma once
#include "Action.h"
#include "MouseManager.h"
#include "AudioManager.h"
#include "FruitThrow.h"
#include "Animation.h"
#include <list>

#define TRAIL_LENGTH 5
#define TRAIL_COLOR 255, 255, 255
#define FRAME_DURATION 0.08
#define MOUSE_ANIMATION_PATHS { L"Images\\stars0.gif", L"Images\\stars1.gif", L"Images\\stars2.gif", L"Images\\stars3.gif", L"Images\\stars4.gif", L"Images\\stars5.gif", L"Images\\stars6.gif", L"Images\\stars7.gif", L"Images\\stars8.gif" }

#define HIT_AUDIO_FILE "Audio\\hit.wav"
#define FRUIT_THROW_ICON L"Images\\fruitThrow.png"

#define MIN_FRUIT_TIMER 0.325
#define MAX_FRUIT_TIMER 0.825
#define FRUIT_COUNT 30

#define MOUSE_DISABLE_TIMER 2
#define MOUSE_DISABLED_ANIMATION_SIZE 64

#define COMPUTATION_SUB_FRAME_INTERVAL 0.006

#define MOUSE_VELOCITY_DIV 5
#define CURSOR_FRICTION -50
// accelaration from friction (px/s^2)

typedef struct
{
    int x;
    int y;
} TrailPoint;

class FruitThrowAction :
    public Action
{
public:
    // FruitThrowAction Constructor
    // input: progress bar requirement, pointer to draw object, mouse manager reference, audio manager reference
    FruitThrowAction(double req, Draw* draw, MouseManager& mouseManager, AudioManager& audioManager);

    // FruitThrowAction Destructor
    ~FruitThrowAction();

    virtual void start();

    virtual void update(double dt);

private:
    // method computes movement of object multiple times per frame
    // input: time since last sub-update, mouse position (x1, y1) in pixels
    // return: none
    void subFrameUpdate(double dt, double x1, double y1);

    Draw* _draw;
    MouseManager& _mouseManager;
    AudioManager& _audioManager;
    double _timer = 0;
    double _fruitLeft = 0;

    double _vx = 0, _vy = 0;
    double _px = 0, _py = 0;
    bool _mouseDisabled = false;
    double _mouseDisabledTimer = 0;

    std::list<FruitThrow*> _currentFruits;
    std::list<TrailPoint> _trailPoints;

    Animation _mouseDisabledAnimation;
};
