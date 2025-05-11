#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pinConfig.h"
#include "all_frames.h"
#define FRAME_DELAY 42 // 24FPS

class Oled
{
private:
    int currentDisplay = 0;
    int _width = screenWidth;
    int _higth = screenHeight;
    int _line[5][2] = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}};

public:
    Adafruit_SSD1306 display;
    Oled();
    void init();
    void clear();
    void setLine(int lowLine[5], int highLine[5]);
    void debug(const String &text, int delayTime = 1000);
    void showText(const String &text, int x = 0, int y = 0, int size = 1);
    void showBar(int percent);
    void showLine(int x1, int y1, int x2, int y2);
    void displayUpdate();
    void setMode(int mode);
    int getMode();
    void printWifiInfo(String ssid, String pass, String ip);
    void printLine(int line[5]);
    void printBattery(float volt);
    void printMotor(int speed[2]);
    void printLineError(float error);
    void printState(SocketState state);
    void hello();

    ~Oled();
};
