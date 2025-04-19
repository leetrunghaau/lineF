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
int* Line::getRawLine()
{
    static int rawLine[5];
    for (int i = 0; i < 5; i++) {
        rawLine[i] = analogRead(_pin[i]);
    }
    return rawLine;
}
int *Line::getLowLine()
{
    return _lowLine;
}
int *Line::getHighLine()
{
    return _highLine;
}
float Line::getLineError()
{
  getLine(); // cập nhật mảng _line[5]

  const float weights[5] = { -2.0, -1.0, 0.0, 1.0, 2.0 };
  float error = 0.0;
  int sum = 0;
  int activeCount = 0;

  for (int i = 0; i < 5; i++) {
    if (_line[i] == 1) {
      error += weights[i];
      sum++;
    }
  }

  // PHÁT HIỆN TÌNH HUỐNG ĐẶC BIỆT
  if (sum == 0) {
    _lineState = LINE_LOST;
    return 0.0;
  }

  if (_line[0] == 1 && _line[4] == 1 && sum == 2) {
    _lineState = LINE_JUNCTION;
    return 0.0; // hoặc return giá trị đặc biệt nếu cần
  }

  if (_line[0] == 1 && sum == 1) {
    _lineState = LINE_HARD_LEFT;
    return -2.5;
  }

  if (_line[4] == 1 && sum == 1) {
    _lineState = LINE_HARD_RIGHT;
    return 2.5;
  }

  // Nếu không phải trường hợp đặc biệt
  float finalError = error / sum;

  if (finalError < -1.0) {
    _lineState = LINE_LEFT;
  } else if (finalError > 1.0) {
    _lineState = LINE_RIGHT;
  } else {
    _lineState = LINE_CENTERED;
  }

  return finalError;
}

LineState Line::getLineState() {
  return _lineState;
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
