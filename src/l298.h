#pragma once

#include "Arduino.h"
#include "pinConfig.h"
#include "battery.h"
#include "oled.h"
#include <HCSR04.h>
#define time45 500
#define time90 1100
#define timeForward 400
#define maxSpeed 255
#define minSpeed 0
#define baseVolt 2.8
class L298
{
private:
    int _t45, _t90, _timeForward ;
    int _enableA;
    int _enableB;
    int _inputA1;
    int _inputA2;
    int _inputB1;
    int _inputB2;
    float _baseVolt;
    Battery _battery;
    void testLeft(int time);
    void testRigh(int time);

public:
    L298();
    void stop();
    void setValue(int t45, int t90, int TF, float baseV);
    int getBaseSpeed();
    int getT90();
    void forwardAvoid(bool sub = false);
    void forward(int speed = 255);
    void backward(int speed = 255);
    void move(int leftSpeed, int rightSpeed);
    void left(int speed = 255);
    void right(int speed = 255);
    void turnLeft90(bool lock = false);
    void turnLeft45();
    void turnRight90(bool lock = false);
    void turnRight45();
    void test(Oled *oled);

    ~L298();
};
