#include "imu.h"
#include <Arduino.h>
#include <math.h>

Imu::Imu() {}

void Imu::init()
{
    if (!_mpu.begin())
    {
        Serial.println("Không tìm thấy MPU6050!");
        while (1)
            delay(10);
    }
    if (!_bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1)
        {
        }
    }

    _mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    _mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    sensors_event_t acc, gyro, temp;
    _mpu.getEvent(&acc, &gyro, &temp);
    _lastTime = millis();
    delay(500);
}

void Imu::update()
{
    sensors_event_t acc, gyro, temp, compass;
    _mpu.getEvent(&acc, &gyro, &temp);
    unsigned long now = millis();
    float dt = (now - _lastTime) / 1000.0;
    _lastTime = now;
    float pitchAcc = atan2(acc.acceleration.y, acc.acceleration.z) * 180 / PI;
    float rollAcc = atan2(-acc.acceleration.x, acc.acceleration.z) * 180 / PI;
    float yawAcc = atan2(acc.acceleration.x, acc.acceleration.y) * 180 / PI;
    _pitch = _alpha * (_pitch + gyro.gyro.x * dt * 180 / PI) + (1 - _alpha) * pitchAcc;
    _roll = _alpha * (_roll + gyro.gyro.y * dt * 180 / PI) + (1 - _alpha) * rollAcc;
    _yaw = _alpha * (_yaw + gyro.gyro.z * dt * 180 / PI) + (1 - _alpha) * yawAcc;
    // _yaw += gyro.gyro.z * dt * 180 / PI;

    _acc[0] = acc.acceleration.x;
    _acc[1] = acc.acceleration.y;
    _acc[2] = acc.acceleration.z;
    _gyro[0] = gyro.gyro.x;
    _gyro[1] = gyro.gyro.y;
    _gyro[2] = gyro.gyro.z;
    _npuTemp = temp.temperature;
    _temp = _bmp.readTemperature();
    _pressure = _bmp.readPressure();
    _altitude = _bmp.readAltitude();
}

float Imu::getYaw()
{
    return _yaw;
}

float Imu::getPitch()
{
    return _pitch;
}

float Imu::getRoll()
{
    return _roll;
}
float *Imu::getAcc()
{
    return _acc;
}
float *Imu::getGyro()
{
    return _gyro;
}
float Imu::getTemp()
{
    return _temp;
}
float Imu::getNpuTemp()
{
    return _npuTemp;
}
int32_t Imu::getPressure()
{
    return _pressure;
}
float Imu::getAltitude()
{
    return _altitude;
}

