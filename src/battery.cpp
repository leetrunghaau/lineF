#include "battery.h"
#include <Arduino.h>
#include "pinConfig.h"

Battery::Battery()
{
}
void Battery::info()
{
    Serial.println("====== Battery Info ======");
    Serial.print("_pinBattery: ");
    Serial.println(_pinBattery);

    Serial.print("_R1: ");
    Serial.println(_R1, 4); // in 4 chữ số sau dấu phẩy (tuỳ chọn)

    Serial.print("_R2: ");
    Serial.println(_R2, 4);

    Serial.print("_fullBattery: ");
    Serial.println(_fullBattery, 2);

    Serial.print("_lowBattery: ");
    Serial.println(_lowBattery, 2);

    Serial.print("_raw: ");
    Serial.println(_raw);

    Serial.print("_batteryVoltage: ");
    Serial.println(_batteryVoltage, 3); // in 3 chữ số sau dấu phẩy

    Serial.print("_batteryLevel: ");
    Serial.println(_batteryLevel, 2);
    Serial.println("==========================");
}
void Battery::init()
{
    _pinBattery = pinBattery;
    _R1 = R1;
    _R2 = R2;
    _fullBattery = hightBattery;
    _lowBattery = lowBattery;
    _raw = 0;
    _batteryVoltage = 0.0;
    _batteryLevel = 0.0;
    pinMode(_pinBattery, INPUT);
    this->info();
}
void Battery::loadBattery()
{

    _raw = analogRead(_pinBattery);
}
float Battery::getBatteryVoltage()
{
    loadBattery();
    _batteryVoltage = (_raw * 3.3 / 4095.0) * (_R1 + _R2) / _R2;
    return _batteryVoltage;
}
float Battery::getBatteryLevel()
{
    loadBattery();
    _batteryLevel = map(_raw, 0, 4095, 0, 100);
    _batteryLevel = constrain(_batteryLevel, 0, 100);
    return _batteryLevel;
}
float Battery::getFullBattery()
{
    return _fullBattery;
}
float Battery::getLowBattery()
{
    return _lowBattery;
}
Battery::~Battery() {}
