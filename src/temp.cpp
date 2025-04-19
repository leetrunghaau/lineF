// #include <Arduino.h>
// #include <HCSR04.h>
// #include "pinConfig.h"
// #include "l298.h"

// UltraSonicDistanceSensor sonic(sonicTrig, sonicEcho);
// L298 motor;



// void setup()
// {
//   Serial.begin(115200);
//   Serial.println(sonic.measureDistanceCm());
//   motor.forward(255);
//   delay(1000);
//   motor.backward(255);
//   delay(1000);
// }

// void loop()
// {
// }

// #include <Arduino.h>
// #include <Wire.h>
// #include "imu.h"
// #include "oled.h"
// #include "l298.h"

// Oled oled;
// Imu myIMU;
// L298 motor(&myIMU);

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(21, 22); // SDA, SCL for ESP32
//   myIMU.init();
//   oled.init();
//   oled.clear();

//   oled.showText("Hello, OLED!", 0, 0, 2);
//   oled.showBar(65); // hiển thị thanh bar 65%
//   oled.displayUpdate();

//   motor.forward(255); // di chuyển tới trước với tốc độ 255
//   delay(2000); // giữ trong 2 giây
//   motor.turnLeftAngle(90, 255); 
//   delay(2000); // giữ trong 2 giây
// }

// void loop() {
  
//   delay(100);
// }
