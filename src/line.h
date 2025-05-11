#pragma once

#include <Arduino.h>
#include "oled.h"
#include "l298.h"
#include "pinConfig.h"
#include "rom.h"




class Line
{
private:
    Rom *_rom;
    bool _line[5];
    int _lowLine[5];
    int _highLine[5];
    int _calibrateLine[5];
    int _pin[5];
    bool _actiton = false;
    LineState _lineState = LINE_CENTERED;
    

public:
    Line();
    void init(Rom *rom);
    void printInfo();
    int *getPin();
    void getLine();
    int *getRawLine();
    int *getLowLine();
    int *getHighLine();
    void setLowLine(int lowLine[5]);
    void updateLowLine(int lowLine[5]);
    void setHighLine(int highLine[5]);
    void updateHighLine(int highLine[5]);
    void updateCalibrateLine();
    void calibrateLine(bool first);
    void saveCalibration();
    float getLineError();
    LineState getLineState();
    ~Line();
};
