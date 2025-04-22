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
    bool _line[5];
    int _lowLine[5];
    int _highLine[5];
    int _calibrateLine[5];
    int _pin[5];
    LineState _lineState = LINE_CENTERED;
    

public:
    Line();
    void setOled(Oled *oled);
    void setMotor(L298 *motor);
    void calibrateLine();
    void initPin();
    int *getPin();
    void getLine();
    int *getRawLine();
    int *getLowLine();
    int *getHighLine();
    void setLowLine(int lowLine[5]);
    void setHighLine(int highLine[5]);
    void updateCalibrateLine();
    float getLineError();
    LineState getLineState();
    bool getLineAt(int index);
    int getRawLineAt(int index);
    ~Line();
};
