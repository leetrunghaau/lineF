#include <WiFi.h>
#include "web.h"
#include "socket.h"
#include "pinConfig.h"
#include "imu.h"
#include "l298.h"
#include "line.h"
#include "oled.h"
#include <HCSR04.h>
#include <Wire.h>
#include "battery.h"
#include "pid.h"

UltraSonicDistanceSensor sonic(sonicTrig, sonicEcho);
Oled oled;
Imu imu;
PID pid;
L298 motor;
Line line;
Web web;
Socket socket;

void lineForward()
{
  float error = line.getLineError();
  int *speeds = pid.compute(error);
  motor.move(speeds[0], speeds[1]);
}

void calibrateLine()
{

  motor.forward();
  unsigned long startTime = millis();
  int lowLine[5] = {4095, 4095, 4095, 4095, 4095};
  int highLine[5]= {0, 0, 0, 0, 0};

  while (millis() - startTime < 2000) 
  {
    oled.debug("getting line...\n " + 2000 - millis(),10);
    for (int i = 0; i < 5; i++)
    {
      int value = line.getRawLineAt(i);
      lowLine[i] = min(lowLine[i], value);
      highLine[i] = max(highLine[i], value);
    }
  }
  line.setLowLine(lowLine);
  line.setHighLine(highLine);
  line.updateCalibrateLine();
}
void setup()
{

  Serial.begin(115200);
  while (!Serial)
    delay(10);
  Wire.begin(21, 22);
  oled.init();
  oled.debug("sensor init...");
  imu.init();
  imu.update();
  motor.setImu(&imu);
  line.setOled(&oled);
  WiFi.softAP(wifiSsid, wifiPassword);
  oled.debug("ssid: " + String(wifiSsid) + "\npas: " + String(wifiSsid) + "\nip: " + WiFi.softAPIP());
  socket.initSocket(&line);
  web.begin(); 
  oled.debug("ssid: " + String(wifiSsid) + "\npas: " + String(wifiSsid) + "\nip: " + WiFi.softAPIP() + "\ndone!",3000);


  oled.debug("line setup");
  oled.debug("3");
  oled.debug("2");
  oled.debug("1");
  oled.debug("lay nguong line");
  calibrateLine();
  oled.debug("line start");
  oled.debug("3");
  oled.debug("2");
  oled.debug("1");
}

void loop()
{

  web.handle();
  socket.soketLoop();
  Serial.println(socket.isConnected());
  if (!socket.isConnected())
  {
    oled.debug("line folow",1);
    lineForward();
  }
  else
  {
    // oled.debug("control",1);
    int run = socket.getRun();
    int turn = socket.getTurn();
    int speed = map(run, -100, 100, -255, 255);
    int turnSpeed = map(turn, -100, 100, -255, 255);
    motor.move(speed + turnSpeed, speed - turnSpeed);
  }
}
