#pragma once

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

class Imu
{
public:
    Imu();
    void init();
    void update();

    float getYaw();
    float getPitch();
    float getRoll();

    float *getAcc();
    float *getGyro();
    float getTemp();
    float getNpuTemp();
    int32_t getPressure();
    float getAltitude();
    float *getMagnetic();
    float getHeading();

private:
    Adafruit_MPU6050 _mpu;
    Adafruit_BMP085 _bmp;
    float _yaw = 0;
    float _pitch = 0;
    float _roll = 0;
    float _acc[3] = {0, 0, 0};
    float _gyro[3] = {0, 0, 0};
    float _temp = 0;
    float _npuTemp = 0;
    int32_t _pressure = 0;
    float _altitude = 0;
    unsigned long _lastTime = 0;
    float _alpha = 0.9; // hệ số lọc cho cảm biến MPU6050



};
