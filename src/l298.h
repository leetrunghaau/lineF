#pragma once

#include "Arduino.h"
#include "pinConfig.h"
#include "imu.h"

class L298
{
private:
    int _enableA;
    int _enableB;
    int _inputA1;
    int _inputA2;
    int _inputB1;
    int _inputB2;
    Imu *_imu;

public:
    L298();
    void stop();
    void forward(int speed = 255);
    void backward(int speed = 255);
    void left(int speed = 255);
    void right(int speed = 255);
    void move(int leftSpeed, int rightSpeed); 
    void turnLeftAngle(int angle, int speed = 120);
    void turnRightAngle(int angle, int speed = 120);
    void setImu(Imu *imuSensor);
    ~L298();
};
