#include "battery.h"
#include <Arduino.h>
#include "pinConfig.h"

Battery::Battery()
{
    int _pinBattery = pinBattery;
    float _R1 = R1;        
    float _R2 = R2;    
    float _fullBattery  = hightBattery;
    float _lowBattery = lowBattery;  
    int _raw    = 0;        
    float _batteryVoltage = 0.0; 
    float _batteryLevel = 0.0;
    pinMode(_pinBattery, INPUT); 
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
