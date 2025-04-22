#include "pid.h"
#include <Arduino.h>

PID::PID() {
  _baseSpeed = baseSpeed; 
  _Kp = Kp;
  _Ki = Ki; 
  _Kd = Kd; 
  _battery = Battery();
  _integral = 0;
  _lastError = 0;
  _maxError = maxError  ;
  _maxBaseSpeed = maxBaseSpeed;
  _minBaseSpeed = minBaseSpeed;
  _lineIsGlitch = 0;
}

int* PID::compute(float lineError) {
  static int motorSpeeds[2];
  if (lineError < -_maxError || lineError > _maxError) {
    //cảnh báo ở chổ này
    _lineIsGlitch ++;
    if (_lineIsGlitch > 5) {
      motorSpeeds[0] = 0;
      motorSpeeds[1] = 0;
      return motorSpeeds;
    } else {
     
      motorSpeeds[0] = _baseSpeed ;
      motorSpeeds[1] = _baseSpeed ;
      return motorSpeeds;
    }
  } else {
    _lineIsGlitch = 0;
  }
  
  int dynamicBaseSpeed = map(abs(lineError), 0, _maxError, _maxBaseSpeed, _minBaseSpeed);
  dynamicBaseSpeed = constrain(dynamicBaseSpeed, _minBaseSpeed, _maxBaseSpeed);
  // PID Calculation
  _integral += lineError;
  float derivative = lineError - _lastError;
  _lastError = lineError;

  float correction = _Kp * lineError + _Ki * _integral + _Kd * derivative;

  int motorLeft = dynamicBaseSpeed - correction;
  int motorRight = dynamicBaseSpeed + correction;

  motorLeft = constrain(motorLeft, 0, 255);
  motorRight = constrain(motorRight, 0, 255);

  motorSpeeds[0] = motorLeft;
  motorSpeeds[1] = motorRight;

  return motorSpeeds;
}

void PID::setKp(float kp) {
  _Kp = kp;
}
void PID::setKi(float ki) {
  _Ki = ki;
}
void PID::setKd(float kd) {
  _Kd = kd;
}
void PID::setBaseSpeed(int speed) {
  _baseSpeed = speed;
}
int PID::getBaseSpeed() {
  return _baseSpeed;
}

PID::~PID(){
  
}