#pragma once

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h>
#include "rom.h"
#include "pinConfig.h"

class Imu
{
public:
    Imu();
    void init(Rom *rom);
    void update();

    void getAzi(int* rs);
    bool compearAzi(int a[2]);
    float getPitch();
    float getRoll();

    float *getAcc();
    float *getGyro();
    float getTemp();
    float getNpuTemp();
    int32_t getPressure();
    float getAltitude();
    int *getCompass();
    void compassCalibrate(Rom *rom);

private:
    Adafruit_MPU6050 _mpu;
    Adafruit_BMP085 _bmp;
    QMC5883LCompass _compass;
    Rom *_rom;
    float _pitch = 0;
    float _roll = 0;
    float _acc[3] = {0, 0, 0};
    float _gyro[3] = {0, 0, 0};
    int _comp[3] = {0, 0, 0};
    float _temp = 0;
    float _npuTemp = 0;
    int32_t _pressure = 0;
    float _altitude = 0;
    unsigned long _lastTime = 0;
    float _alpha = 0.9; // hệ số lọc cho cảm biến MPU6050
    float _calibrationOffsets[3];
    float _calibrationScales[3]; 
};
