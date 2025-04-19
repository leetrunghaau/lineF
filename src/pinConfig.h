#pragma once
// sonic
#define sonicTrig 13 
#define sonicEcho 12 

// l298
#define enableAPin 0
#define enableBPin 0
#define inputA1Pin 0
#define inputA2Pin 0
#define inputB1Pin 0
#define inputB2Pin 0

// i2c address 
#define QMC5883L_ADDRESS 0x0D
#define MPU6050_ADDRESS 0x68
#define BMP180_ADDRESS 0x1E
#define SSD1306_ADDRESS 0x00

//oled
#define screenWidth 128
#define screenHeight 64
#define oledReset -1


//line
#define line1Pin 34
#define line2Pin 35
#define line3Pin 32
#define line4Pin 33
#define line5Pin 25



//wifi
#define wifiSsid "hau"
#define wifiPassword "12345679"

// pid parameters
const float Kp = 30.0;
const float Ki = 0.0;
const float Kd = 5.0;


// pid battery
#define R1 10000.0
#define R2 10000.0
#define R3 10000.0
const int pinBattery = 36;