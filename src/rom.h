#pragma once
#include <Arduino.h>
#include <EEPROM.h>

class Rom {
private:
    int _lowLine[5];
    int _highLine[5];
    float _calibrationOffsets[3];
    float _calibrationScales[3];

    const int EEPROM_SIZE = 100;
    const int FLAG_ADDRESS = 0;
    const byte VALID_FLAG = 0xA5;

    void loadDefaults();
    void printInfo();

public:
    Rom();
    ~Rom();
    void init();
    int* getLowLine();
    void setLowLine(int lowLine[5]);

    int* getHighLine();
    void setHighLine(int highLine[5]);

    float* getCalibrationOffsets();
    void setCalibrationOffsets(float calibrationOffsets[3]);

    float* getCalibrationScales();
    void setCalibrationScales(float calibrationScales[3]);

    void save();
    void load();
};
