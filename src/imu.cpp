#include "imu.h"
#include <math.h>
#include <Arduino.h>


Imu::Imu() {}

void Imu::init()
{
    compass.init();
    mpu.initialize();
    delay(100);
    calibrateGyro();
    _lastTime = millis();
}

void Imu::calibrateGyro()
{
    long sum = 0;
    const int samples = 500;

    for (int i = 0; i < samples; i++)
    {
        int16_t gz;
        mpu.getRotation(nullptr, nullptr, &gz);
        sum += gz;
        delay(2);
    }

    _gyroZ_offset = sum / (float)samples;
}

void Imu::update()
{
    // Time
    unsigned long now = millis();
    float dt = (now - _lastTime) / 1000.0;
    _lastTime = now;

    // Raw values
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Normalize accel
    float axg = ax / 16384.0;
    float ayg = ay / 16384.0;
    float azg = az / 16384.0;

    // Calculate pitch, roll
    _pitch = atan2(axg, sqrt(ayg * ayg + azg * azg)) * 180 / PI;
    _roll  = atan2(ayg, azg) * 180 / PI;

    // Gyro yaw estimate
    float gyroZ = (gz - _gyroZ_offset) / 131.0;
    _yaw += gyroZ * dt;

    // Magnetometer heading
    compass.read();
    int mx = compass.getX();
    int my = compass.getY();
    float magYaw = atan2((float)my, (float)mx) * 180 / PI;
    if (magYaw < 0) magYaw += 360;

    // Complementary filter
    _yaw = alpha * _yaw + (1 - alpha) * magYaw;
    if (_yaw > 360) _yaw -= 360;
    if (_yaw < 0) _yaw += 360;
}

float Imu::getYaw() { return _yaw; }
float Imu::getPitch() { return _pitch; }
float Imu::getRoll() { return _roll; }