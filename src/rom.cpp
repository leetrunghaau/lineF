#include "Rom.h"
// #include <nvs_flash.h>

Rom::Rom()
{
}

Rom::~Rom() {}

void Rom::init()
{

    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //   Serial.println("Formatting NVS...");
    //   nvs_flash_erase();
    //   nvs_flash_init();
    // }

    if (!EEPROM.begin(EEPROM_SIZE))
    {
        Serial.println("Failed to initialize EEPROM");
        delay(1000);
        ESP.restart();
    }
    if (EEPROM.read(FLAG_ADDRESS) == VALID_FLAG)
    {
        load();
        Serial.println("EEPROM data loaded.");
        
    }
    else
    {
        Serial.println("EEPROM not initialized, loading defaults.");
        loadDefaults();
        save(); // Save defaults to EEPROM
    }
    this->printInfo();
}

void Rom::loadDefaults()
{
    int lowLineDefaults[5] = {10, 20, 30, 40, 50};
    int highLineDefaults[5] = {60, 70, 80, 90, 100};
    float offsetDefaults[3] = {-3543.00, 546.00, -4798.00};
    float scaleDefaults[3] = {0.90, 2.53, 0.67};

    this->setLowLine(lowLineDefaults);
    this->setHighLine(highLineDefaults);
    this->setCalibrationOffsets(offsetDefaults);
    this->setCalibrationScales(scaleDefaults);
}
void Rom::printInfo()
{
    Serial.println("====== Line in rom Info ======");
    String str1 = "";
    String str2 = "";
    for (int i = 0; i < 5; i++)
    {
        str1 += String(_lowLine[i]) += " | ";
        str2 += String(_highLine[i]) += " | ";
    }

    Serial.println("low line: " + str1);
    Serial.println("lhigh line: " + str2);
    Serial.println("==========================");
}

void Rom::save()
{
    int addr = FLAG_ADDRESS + 1;

    for (int i = 0; i < 3; i++)
    {
        EEPROM.put(addr, _calibrationOffsets[i]);
        addr += sizeof(float);
    }

    for (int i = 0; i < 3; i++)
    {
        EEPROM.put(addr, _calibrationScales[i]);
        addr += sizeof(float);
    }

    for (int i = 0; i < 5; i++)
    {
        EEPROM.put(addr, _lowLine[i]);
        addr += sizeof(int);
    }

    for (int i = 0; i < 5; i++)
    {
        EEPROM.put(addr, _highLine[i]);
        addr += sizeof(int);
    }

    EEPROM.write(FLAG_ADDRESS, VALID_FLAG);
    EEPROM.commit();
    Serial.println("EEPROM data saved.");
}

void Rom::load()
{
    int addr = FLAG_ADDRESS + 1;

    for (int i = 0; i < 3; i++)
    {
        EEPROM.get(addr, _calibrationOffsets[i]);
        addr += sizeof(float);
    }

    for (int i = 0; i < 3; i++)
    {
        EEPROM.get(addr, _calibrationScales[i]);
        addr += sizeof(float);
    }

    for (int i = 0; i < 5; i++)
    {
        EEPROM.get(addr, _lowLine[i]);
        addr += sizeof(int);
    }

    for (int i = 0; i < 5; i++)
    {
        EEPROM.get(addr, _highLine[i]);
        addr += sizeof(int);
    }
}

int *Rom::getLowLine()
{
    return _lowLine;
}

void Rom::setLowLine(int lowLine[5])
{
    for (int i = 0; i < 5; i++)
    {
        _lowLine[i] = lowLine[i];
    }
}

int *Rom::getHighLine()
{
    return _highLine;
}

void Rom::setHighLine(int highLine[5])
{
    for (int i = 0; i < 5; i++)
    {
        _highLine[i] = highLine[i];
    }
}

float *Rom::getCalibrationOffsets()
{
    return _calibrationOffsets;
}

void Rom::setCalibrationOffsets(float calibrationOffsets[3])
{
    for (int i = 0; i < 3; i++)
    {
        _calibrationOffsets[i] = calibrationOffsets[i];
    }
}

float *Rom::getCalibrationScales()
{
    return _calibrationScales;
}

void Rom::setCalibrationScales(float calibrationScales[3])
{
    for (int i = 0; i < 3; i++)
    {
        _calibrationScales[i] = calibrationScales[i];
    }
}
