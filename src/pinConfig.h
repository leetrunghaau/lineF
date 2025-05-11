#pragma once
#include <Arduino.h>
// 🟢 SONIC
#define sonicTrig 18 // 🟢
#define sonicEcho 19 // 🟢

// 🟢 L298 – Motor Driver
#define enableAPin 27 // 🟢
#define enableBPin 14 // 🟢
#define inputA1Pin 26 // 🟢
#define inputA2Pin 25 // 🟢
#define inputB1Pin 4  // 🟢
#define inputB2Pin 15 // 🟢

// 🟢 I2C
#define I2C_SDA 21 // 🟢
#define I2C_SCL 22 // 🟢

// 🟢 OLED & Sensor Address
#define QMC5883L_ADDRESS 0x67
#define MPU6050_ADDRESS 0x68
#define BMP180_ADDRESS 0x1E
#define SSD1306_ADDRESS 0x3C

#define minX -6200
#define maxX -3200
#define minY 18300
#define maxY 20750

// 🟢 OLED Config
#define screenWidth 128
#define screenHeight 64
#define oledReset -1

// 🟢 buzzer
#define buzzerPin 23 //   🟢
#define ledPin 2     //

// 🟢 LINE SENSOR – Analog Input Only
#define line1Pin 36 // 🟢
#define line2Pin 39 // 🟢
#define line3Pin 34 // 🟢
#define line4Pin 35 // 🟢
#define line5Pin 32 // 🟢

// 🟢 WiFi
#define wifiSsid "hau"
#define wifiPassword "12345679"




// 🟢 Battery ADC
#define pinBattery 33 // 🟢
#define R1 20000.0
#define R2 5000.0
#define hightBattery 16.8
#define lowBattery 14.8

enum LineState
{
  LINE_LOST,
  LINE_CENTERED,
  LINE_LEFT,
  LINE_RIGHT,
  LINE_JUNCTION,
  LINE_HARD_LEFT,
  LINE_HARD_RIGHT,
  LINE_GLITCH
};

enum SocketState
{
  DISCONNECTED,
  LINE_FOLOW,
  CONTROL,
  START_CALIBRATE,
  CALIBRATTING,
  END_CALIBRATE,
};

extern String SocketStateName[];