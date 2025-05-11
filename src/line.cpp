#include "line.h"

Line::Line()
{
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

void Line::updateLowLine(int lowLine[5])
{

  for (int i = 0; i < 5; i++)
  {
    _lowLine[i] = min(_lowLine[i], lowLine[i]);
  }
}
void Line::updateHighLine(int highLine[5])
{
  for (int i = 0; i < 5; i++)
  {
    _highLine[i] = max(_highLine[i], highLine[i]);
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
    return 5.0;
  }
  if (sum == 5)
  {
    _lineState = LINE_JUNCTION;
    return 0.0;
  }
  // if (sum >= 2)
  // {
  //   int lastIndex = 0;
  //   for (int i = 0; i < 5; i++)
  //   {
  //     lastIndex = _line[i] ? i : lastIndex;
  //     for (int j = i; j < 5; j++)
  //     {
  //       if (_line[j])
  //       {
  //         if (j - i > 1)
  //         {
  //           _lineState = LINE_GLITCH;
  //           return 100;
  //         }
  //         break;
  //       }
  //     }
  //   }
  // }
  float finalError = error / sum;
  _lineState = finalError < -0.5 ? LINE_LEFT : (finalError > 0.5 ? LINE_RIGHT : LINE_CENTERED);
  return finalError;
}
LineState Line::getLineState()
{
  return _lineState;
}

void Line::calibrateLine(bool first)
{
  if (first)
  {
    int *raw = this->getRawLine();
    for (int i = 0; i < 5; i++)
    {
      _lowLine[i] = raw[i];
      _highLine[i] = raw[i];
    }
  }
  else
  {
    int *raw = this->getRawLine();
    this->updateLowLine(raw);
    this->updateHighLine(raw);
  }
}
void Line::saveCalibration()
{
  _rom->setLowLine(_lowLine);
  _rom->setHighLine(_highLine);
  _rom->save();
}
void Line::init(Rom *rom)
{
  _rom = rom;
  _pin[0] = line1Pin;
  _pin[1] = line2Pin;
  _pin[2] = line3Pin;
  _pin[3] = line4Pin;
  _pin[4] = line5Pin;
  int *lowLineInit = _rom->getLowLine();
  int *highLineInit = _rom->getHighLine();

  for (int i = 0; i < 5; i++)
  {
    pinMode(_pin[i], INPUT_PULLUP);
    _lowLine[i] = lowLineInit[i];
    _highLine[i] = highLineInit[i];
    _calibrateLine[i] = 512;
    _line[i] = false;
  }
  this->updateCalibrateLine();
  this->printInfo();
}
void Line::printInfo()
{
  Serial.println("===== Line Sensor Info =====");

  Serial.print("Pins: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(_pin[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Raw: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(_line[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Low: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(_lowLine[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("High: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(_highLine[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Calibrated: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(_calibrateLine[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Line State: ");
  switch (_lineState)
  {
  case LINE_LEFT:
    Serial.println("LEFT");
    break;
  case LINE_RIGHT:
    Serial.println("RIGHT");
    break;
  case LINE_CENTERED:
    Serial.println("CENTERED");
    break;
  case LINE_LOST:
    Serial.println("LOST");
    break;
  default:
    Serial.println("UNKNOWN");
    break;
  }

  Serial.print("Line Error: ");
  Serial.println(getLineError(), 2);
}

int *Line::getPin()
{
  return _pin;
}
Line::~Line()
{
}
