#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pinConfig.h"

class Oled
{
private:
    Adafruit_SSD1306 display;
    int currentDisplay = 0;
    int _width = screenWidth;
    int _higth = screenHeight;

public:
    Oled();
    void init();
    void clear();
    void debug(const String &text, int delayTime = 1000);
    void showText(const String &text, int x = 0, int y = 0, int size = 1);
    void showBar(int percent);
    void showLine(int x1, int y1, int x2, int y2);
    void displayUpdate();
    void setMode(int mode);
    int getMode();

    ~Oled();
};
