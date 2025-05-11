#pragma once



class Battery
{
private:
    int _pinBattery;
    float _R1;         
    float _R2;         
    float _fullBattery ; 
    float _lowBattery;  
    int _raw;        
    float _batteryVoltage; 
    float _batteryLevel;   
    void loadBattery();
    

public:
    Battery();
    void init();
    void info();
    ~Battery();
    float getBatteryVoltage();            
    float getBatteryLevel();              
    float getFullBattery() ;
    float getLowBattery();
   
};


