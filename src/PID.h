#pragma once


#include "battery.h"
#include "pinConfig.h"

class PID
{
public:
  PID();
  int *compute(float lineError);
  void setKp(float kp) ;
  void setKi(float ki) ;
  void setKd(float kd) ;
  void setBaseSpeed(int speed) ;
  int getBaseSpeed() ;
  int _lineIsGlitch;
  ~PID();
private:
  Battery _battery;
  int _baseSpeed ;
  float _Kp ;
  float _Ki ;
  float _Kd ;
  float _maxError;
  int _maxBaseSpeed;
  int _minBaseSpeed;
  float _integral, _lastError;
};