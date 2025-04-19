#include "line.h"


Line::Line()
{
    _pin[0] = line1Pin;
    _pin[1] = line2Pin;
    _pin[2] = line3Pin;
    _pin[3] = line4Pin;
    _pin[4] = line5Pin;

    this->initPin();

    _oled = nullptr;
    _motor = nullptr;
}
Line::Line(Oled *oled, L298 *motor)
{
    _pin[0] = line1Pin;
    _pin[1] = line2Pin;
    _pin[2] = line3Pin;
    _pin[3] = line4Pin;
    _pin[4] = line5Pin;

    this->initPin();
    _oled = oled;
    _motor = motor;
}
Line::Line(Oled *oled, L298 *motor, int pin1, int pin2, int pin3, int pin4, int pin5)
{
    _pin[0] = pin1;
    _pin[1] = pin2;
    _pin[2] = pin3;
    _pin[3] = pin4;
    _pin[4] = pin5;

    this->initPin();

    _oled = oled;
    _motor = motor;
}
void Line::setOled(Oled *oled)
{
    _oled = oled;
}
void Line::setMotor(L298 *motor)
{
    _motor = motor;
}
void Line::calibrateLine()
{
    if (_motor == nullptr)
    {
        Serial.println("Motor is not set");
        return;
    }
    if (_oled) {
        _oled->showText("Calibrating...");
    }
    
    _motor->forward();
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) // chạy trong 5 giây
    {
        for (int i = 0; i < 5; i++)
        {
            int value = analogRead(_pin[i]);
            _lowLine[i] = min(_lowLine[i], value);
            _highLine[i] = max(_highLine[i], value);
            _calibrateLine[i] = (_lowLine[i] + _highLine[i]) / 2;
        }
    }
}
int *Line::getLine()
{
    for (int i = 0; i < 5; i++)
    {
        int value = analogRead(_pin[i]);
        _line[i] = value > _calibrateLine[i] ? 1 : 0; 
    }
    return _line;
}
int Line::getLineAt(int index) {
    if (index < 0 || index >= 5) return -1;
    int value = analogRead(_pin[index]);
    return value > _calibrateLine[index] ? 1 : 0;
}

void Line::initPin()
{
    for (int i = 0; i < 5; i++)
    {
        pinMode(_pin[i], INPUT_PULLUP);
        _lowLine[i] = 1024;
        _highLine[i] = 0;
        _calibrateLine[i] = 512;
        _line[i] = 0;
    }
}
Line::~Line()
{
}
