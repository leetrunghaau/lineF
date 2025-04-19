#pragma once

#include <QMC5883LCompass.h>
#include "I2Cdev.h"
#include "MPU6050.h"

class Imu
{
private:
    QMC5883LCompass compass;
    MPU6050 mpu;

    float _yaw = 0;
    float _pitch = 0;
    float _roll = 0;
    float _gyroZ_offset = 0;

    unsigned long _lastTime = 0;
    const float alpha = 0.98; 

public:
    Imu();
    void init();
    void calibrateGyro();
    void update();

    float getYaw();
    float getPitch();
    float getRoll();
};
