#include "oled.h"
#include <Wire.h>

Oled::Oled() : display(screenWidth, screenHeight, &Wire, oledReset) {}

void Oled::init()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS))
    {
        Serial.println(F("SSD1306 initialization failed!"));
        while (true)
            ;
    }

    display.clearDisplay();
    display.display();
    this->debug("Hello, I'm robot!");
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
void Oled::setLine(int lowLine[5], int highLine[5])
{
    for (int i = 0; i < 5; i++)
    {
        _line[i][0] = lowLine[i];
        _line[i][1] = highLine[i];
    }
}

void Oled::printLine(int line[5])
{
    String str = "";
    for (int i = 4; i >=0; i--)
    {
        str += ((_line[i][0] + _line[i][1]) / 2) > line[i] ? "#" : "_";
    }
    display.setCursor(48, 14);
    display.print(str);
    
}

void Oled::printWifiInfo(String ssid, String pass, String ip)
{
    // print |
    display.fillRect((128 / 2) - 1, 49, 2, 8, SSD1306_WHITE);
    //  print ssid
    display.setCursor(0, 47);
    display.print(ssid);
    // print pass
    display.setCursor((128 - (pass.length() * 6)), 47);
    display.print(pass);
    // print ip
    display.setCursor((128 - (ip.length() * 6)) / 2, 56);
    display.print(ip);
}
void Oled::printBattery(float volt)
{
    String rs = String(volt) + "V";
    display.setCursor(92, 0);
    display.print(rs);
}
void Oled::showBar(int percent)
{
    percent = constrain(percent, 0, 100);
    int barWidth = map(percent, 0, 100, 0, screenWidth);
    display.drawRect(0, 25, screenWidth, 10, SSD1306_WHITE); // khung bar
    display.fillRect(0, 25, barWidth, 10, SSD1306_WHITE);    // phần đầy
    this->displayUpdate();
}
void Oled::printState(SocketState state)
{
    display.setCursor(0, 0);
    display.print(SocketStateName[state]);
}

void Oled::showLine(int x1, int y1, int x2, int y2)
{
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}
void Oled::hello()
{
    int currentFrame = 0;
    unsigned long lastFrameTime = millis();
    while (currentFrame < 89)
    {
        if (millis() - lastFrameTime >= FRAME_DELAY)
        {
            lastFrameTime = millis();
            display.clearDisplay();
            display.drawBitmap(0, 0, frames[currentFrame], FRAME_WIDTH, FRAME_HEIGHT, SSD1306_WHITE);
            display.display();
            currentFrame = (currentFrame + 1);
        }
    }
    delay(1400);
}
void Oled::printMotor(int speed[2])
{
    String r = String(speed[1]);
    display.setCursor(6, 27);
    display.print(String(speed[0]));
    display.setCursor(122 - (r.length() * 6), 27);
    display.print(String(speed[1]));
}
void Oled::printLineError(float error)
{
    display.setTextSize(2);
    String str = String(error);
    display.setCursor((128 - (str.length() * 12)) / 2, 27);
    display.print(String(error));
    display.setTextSize(1);
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