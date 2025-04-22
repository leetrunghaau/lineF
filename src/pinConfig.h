#pragma once
// 🟢 SONIC
#define sonicTrig 18 // 🟢
#define sonicEcho 19 // 🟢

// 🟢 L298 – Motor Driver
#define enableAPin 14 // PWM            // 🟢
#define enableBPin 27 // PWM            // 🟢
#define inputA1Pin 26 // 🟢
#define inputA2Pin 25 // 🟢
#define inputB1Pin 33 // 🟢
#define inputB2Pin 4 // 🟢

// 🟢 I2C
#define I2C_SDA 21 // 🟢
#define I2C_SCL 22 // 🟢

// 🟢 OLED & Sensor Address
#define QMC5883L_ADDRESS 0x67 // bị hư
#define MPU6050_ADDRESS 0x68
#define BMP180_ADDRESS 0x1E
#define SSD1306_ADDRESS 0x3C

// 🟢 OLED Config
#define screenWidth 128
#define screenHeight 64
#define oledReset -1

// 🟢 buzzer
#define buzzerPin 23 // Buzzer Pin     //   🟢
#define ledPin 2     // LED Pin        //

// 🟢 LINE SENSOR – Analog Input Only
#define line1Pin 36 // VP             // 🟢
#define line2Pin 39 // VN             // 🟢
#define line3Pin 34 // 🟢
#define line4Pin 35 // 🟢
#define line5Pin 32  // 🟢



// 🟢 WiFi
#define wifiSsid "hau"
#define wifiPassword "12345679"

// 🧠 PID
#define Kp 30.0
#define Ki 0.0
#define Kd 5.0
#define maxSpeed 255
#define minSpeed 0
#define baseSpeed 200
#define maxBaseSpeed 255
#define minBaseSpeed 0
#define maxError 2.0

// 🟢 Battery ADC
#define pinBattery 13 // ADC OK         // 🟢
#define R1 20000.0
#define R2 5000.0
#define hightBattery 12.6
#define lowBattery 11.1


enum LineState {
    LINE_LOST,
    LINE_CENTERED,
    LINE_LEFT,
    LINE_RIGHT,
    LINE_JUNCTION,
    LINE_HARD_LEFT,
    LINE_HARD_RIGHT,
    LINE_GLITCH
  };
