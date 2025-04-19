#include <WiFi.h>
#include "web.h"
#include "socket.h"
#include "pinConfig.h"
#include "imu.h"
#include "l298.h"
#include "line.h"
#include "oled.h"
#include <HCSR04.h>
#include "SmartPID.h"

String status = "init"; // trạng thái mặc định là "line". line || control || init

UltraSonicDistanceSensor sonic(sonicTrig, sonicEcho);
Oled oled;
Imu imu;
SmartPID smartPID(&imu,pinBattery);
L298 motor(&imu);
Line line(&oled, &motor);
Web web;
Socket socket;

void bar()
{

  // tìm hướng tránh vật cản
  String direction = "left";
  motor.turnLeftAngle(45, 200);
  delay(1000);
  int distance = sonic.measureDistanceCm();

  if (distance < 20)
  {
    direction = "right";
    motor.turnRightAngle(90, 200);
    delay(1000);
    distance = sonic.measureDistanceCm();
  }
  if (distance < 20)
  {
    direction = "backward";
    motor.backward(200);
    delay(1000);
    distance = sonic.measureDistanceCm();
  }
  if (distance < 20)
  {
    direction = "stop";
  }
}

void lineForward()
{
  smartPID.update();

  float error = line.getLineError();
  LineState state = line.getLineState();

  if (state == LINE_LOST)
  {
    motor.stop();
    Serial.println(" Mất line!");
    return;
  }

  if (state == LINE_JUNCTION)
  {
    motor.stop();
    Serial.println(" Gặp ngã ba!");
    return;
  }
  int correction = smartPID.compute(error);
  int baseSpeed = smartPID.getBaseSpeed();

  int left = constrain(baseSpeed - correction, -255, 255);
  int right = constrain(baseSpeed + correction, -255, 255);

  motor.move(left, right);
}
void setup()
{

  // khởi tạo serial
  Serial.begin(115200);
  delay(1000);
  // khởi tạo các cảm biến
  oled.init();
  imu.init();

  // lấy ngưỡng cảm biến line
  line.calibrateLine();

  // setup wifi
  Serial.println("khởi động ESP32...");
  WiFi.softAP(wifiSsid, wifiPassword);

  // setup web
  web.begin();
  socket.initSocket(&line);
  status = "line";

  smartPID.setPID(Kp, Ki, Kd);
  smartPID.setTargetYaw(imu.getYaw());
}

void loop()
{

  web.handle();
  socket.soketLoop();
  status = socket.isConnected() ? "control" : "line";

  if (status == "line")
  {
    lineForward();
  }
  else if (status == "control")
  {
    int run = socket.getRun();
    int turn = socket.getTurn();
    int speed = map(run, -100, 100, -255, 255);
    int turnSpeed = map(turn, -100, 100, -255, 255);
    motor.move(speed + turnSpeed, speed - turnSpeed);
  }
  
}
