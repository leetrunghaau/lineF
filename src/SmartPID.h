#pragma once

#ifndef SMART_PID_H
#define SMART_PID_H

#include "imu.h"

class SmartPID {
public:
  SmartPID(Imu* _imu, int _pinBattery);
  
  void setPID(float kp, float ki, float kd);
  void setTargetYaw(float yaw);
  void update();                      // gọi mỗi vòng lặp để cập nhật deltaYaw
  int compute(float lineError);        // trả về correction
  
  float getDeltaYaw();               // xem deltaYaw nếu cần debug
  int getBaseSpeed();                // tốc độ điều chỉnh theo yaw

private:
  Imu* imu;
  int pinBattery;
  float Kp, Ki, Kd;
  float integral, lastError;
  float targetYaw;

  // --- deltaYaw logic ---
  float lastYaw;
  float deltaYaw;
  unsigned long lastYawCheck;
  
  float getBatteryVoltage();
};

#endif
