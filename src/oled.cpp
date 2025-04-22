#include "oled.h"
#include <Wire.h>

Oled::Oled() : display(screenWidth, screenHeight, &Wire, oledReset) {}

void Oled::init()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS)) {
        Serial.println(F("SSD1306 initialization failed!"));
        while (true);
    }

    display.clearDisplay();
    display.display();
    this->debug("Hello to my robot!");
    delay(1000);
}

void Oled::clear()
{
    display.clearDisplay();
}
void Oled::debug(const String &text, int delayTime)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(text);
    display.display();
    Serial.println(text);
    delay(delayTime);
}

void Oled::showText(const String &text, int x, int y, int size)
{
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.println(text);
}

void Oled::showBar(int percent)
{
    percent = constrain(percent, 0, 100);
    int barWidth = map(percent, 0, 100, 0, screenWidth);
    display.drawRect(0, 30, screenWidth, 10, SSD1306_WHITE); // khung bar
    display.fillRect(0, 30, barWidth, 10, SSD1306_WHITE);    // phần đầy
}

void Oled::showLine(int x1, int y1, int x2, int y2)
{
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}

void Oled::displayUpdate()
{
    display.display();
}

void Oled::setMode(int mode)
{
    currentDisplay = mode;
}

int Oled::getMode()
{
    return currentDisplay;
}

Oled::~Oled()
{
}