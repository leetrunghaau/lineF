#pragma once

#include <Arduino.h>
#include "oled.h"
#include "l298.h"
#include "pinConfig.h"

class Line
{
private:
    Oled *_oled;
    L298 *_motor;
    int _line[5];
    int _lowLine[5];
    int _highLine[5];
    int _calibrateLine[5];
    int _pin[5];

    void initPin();

public:
    Line();
    Line(Oled *oled = nullptr, L298 *motor = nullptr);
    Line(Oled *oled, L298 *motor, int pin1, int pin2, int pin3, int pin4, int pin5);
    void setOled(Oled *oled);
    void setMotor(L298 *motor);
    void calibrateLine();
    int *getLine();
    int getLineAt(int index);
    ~Line();
};
