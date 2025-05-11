#pragma once

#include "pinConfig.h"
#include "l298.h"
#include <HCSR04.h>
#include "oled.h"
#include "line.h"
#define Kp 7.0
#define Ki 0.3
#define Kd 0.0
#define maxError 2.0

class Logic
{
public:
  Logic();
  void init();
  int *compute(float lineError, int baseSpeed);
  int *computeFE(float lineError, int baseSpeed);
  int getBaseSpeed();
  void startLogic();
  float getLeftDistance(L298 *motor, UltraSonicDistanceSensor *sonic);
  float getRightDistance(L298 *motor, UltraSonicDistanceSensor *sonic);
  bool avoidObstacle(L298 *motor, UltraSonicDistanceSensor *sonic,  Oled *oled, Line *line);
  void splitLine(bool direction, L298 *motor, Oled *oled);
  void parallelLine(bool direction, L298 *motor, Oled *oled);
  bool mergeLine(bool direction, L298 *motor, Oled *oled, Line *line);
  ~Logic();

private:
  float _Kp;
  float _Ki;
  float _Kd;
  int _lineIsGlitch;
  float _integral, _lastError;
};