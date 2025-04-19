#include "SmartPID.h"
#include <Arduino.h>
#include "pinConfig.h"

SmartPID::SmartPID(Imu* _imu, int _pinBattery)
  : imu(_imu), pinBattery(_pinBattery),
    Kp(0), Ki(0), Kd(0), integral(0), lastError(0), targetYaw(0),
    lastYaw(0), deltaYaw(0), lastYawCheck(0) {}

void SmartPID::setPID(float kp, float ki, float kd) {
  Kp = kp;
  Ki = ki;
  Kd = kd;
}

void SmartPID::setTargetYaw(float yaw) {
  targetYaw = yaw;
}

float SmartPID::getBatteryVoltage() {
  int raw = analogRead(pinBattery);
  float voltage = (raw * 3.3 / 4095.0) * (R1 + R2) / R2; // thay R1 R2 tùy chia áp
  return voltage;
}

void SmartPID::update() {
  unsigned long now = millis();
  if (now - lastYawCheck >= 100) { // cập nhật mỗi 100ms
    float currentYaw = imu->getYaw();
    deltaYaw = currentYaw - lastYaw;

    if (deltaYaw > 180) deltaYaw -= 360;
    if (deltaYaw < -180) deltaYaw += 360;

    lastYaw = currentYaw;
    lastYawCheck = now;
  }
}

int SmartPID::getBaseSpeed() {
  return (abs(deltaYaw) > 3.0) ? 120 : 150;
}

float SmartPID::getDeltaYaw() {
  return deltaYaw;
}

int SmartPID::compute(float lineError) {
  float batteryVoltage = getBatteryVoltage();
  float voltageFactor = batteryVoltage / 7.4;

  float currentYaw = imu->getYaw();
  float yawError = targetYaw - currentYaw;
  
  integral += lineError;
  float derivative = lineError - lastError;
  lastError = lineError;

  // Giảm Kp nếu cua gắt
  float adjKp = (abs(deltaYaw) > 3.0) ? Kp * 0.6 : Kp;

  float correction = (adjKp * lineError + Ki * integral + Kd * derivative) * voltageFactor;

  return correction + yawError * 2.0; // bonus chỉnh hướng (nếu cần)
}
