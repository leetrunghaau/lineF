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
  if (_oled)
  {
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
void Line::getLine()
{
  for (int i = 0; i < 5; i++)
  {
    int value = analogRead(_pin[i]);
    _line[i] = value > _calibrateLine[i] ? true : false;
  }
}
int *Line::getRawLine()
{
  static int rawLine[5];
  for (int i = 0; i < 5; i++)
  {
    rawLine[i] = analogRead(_pin[i]);
  }
  _oled->debug(
    String(_lowLine[0]) + " " + String(rawLine[0]) + " " + String(_highLine[0]) + "\n" +
        String(_lowLine[1]) + " " + String(rawLine[1]) + " " + String(_highLine[1]) + "\n" +
        String(_lowLine[2]) + " " + String(rawLine[2]) + " " + String(_highLine[2]) + "\n" +
        String(_lowLine[3]) + " " + String(rawLine[3]) + " " + String(_highLine[3]) + "\n" +
        String(_lowLine[4]) + " " + String(rawLine[4]) + " " + String(_highLine[4]) + "\n",
    20);
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
void Line::setLowLine(int lowLine[5])
{
  for (int i = 0; i < 5; i++)
  {
    _lowLine[i] = lowLine[i];
  }
}
void Line::setHighLine(int highLine[5])
{
  for (int i = 0; i < 5; i++)
  {
    _highLine[i] = highLine[i];
  }
}
void Line::updateCalibrateLine()
{
  for (int i = 0; i < 5; i++)
  {
    _calibrateLine[i] = (_lowLine[i] + _highLine[i]) / 2;
  }
}
float Line::getLineError()
{
  getLine();
  const float weights[5] = {-2.0, -1.0, 0.0, 1.0, 2.0};
  float error = 0.0;
  int sum = 0;


  for (int i = 0; i < 5; i++)
  {
    if (_line[i])
    {
      error += weights[i];
      sum++;
    }
  }

  if (sum == 0)
  {
    _lineState = LINE_LOST;
    return 0.0;
  }
  if (sum == 5)
  {
    _lineState = LINE_JUNCTION;
    return 0.0;
  }

  if (sum >= 2)
  {
    int lastIndex = 0;
    for (int i = 0; i < 5; i++)
    {
      lastIndex = _line[i] ? i : lastIndex;
      for (int j = i; j < 5; j++)
      {
        if (_line[j])
        {
          if (j - i > 1)
          {
            _lineState = LINE_GLITCH;
            return 5.0;
          }
          break;
        }
      }
    }
  }
  float finalError = error / sum;
  _lineState = finalError < -0.5 ? LINE_LEFT : (finalError > 0.5 ? LINE_RIGHT : LINE_CENTERED);
  return finalError;
}
LineState Line::getLineState()
{
  return _lineState;
}
bool Line::getLineAt(int index)
{
  if (index < 0 || index >= 5)
    return -1;
  int value = analogRead(_pin[index]);
  return value > _calibrateLine[index] ? 1 : 0;
}

int Line::getRawLineAt(int index)
{
  if (index < 0 || index >= 5)
    return -1;
  return analogRead(_pin[index]);
}

void Line::initPin()
{
  for (int i = 0; i < 5; i++)
  {
    pinMode(_pin[i], INPUT_PULLUP);
    _lowLine[i] = 1024;
    _highLine[i] = 0;
    _calibrateLine[i] = 512;
    _line[i] = false;
  }
}
int *Line::getPin()
{
  return _pin;
}
Line::~Line()
{
}
