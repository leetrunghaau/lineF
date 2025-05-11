
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
#include "logic.h"
#include "rom.h"

UltraSonicDistanceSensor sonic(sonicTrig, sonicEcho);
Oled oled;
Imu imu;
Logic lg;
L298 motor;
Line line;
Web web;
Socket socket;
Battery battery;
Rom rom;
SocketState pevState = DISCONNECTED;

void lineForward()
{
  float dist = sonic.measureDistanceCm();

  if (dist < 17.0)
  {
    bool ok = lg.avoidObstacle(&motor, &sonic, &oled, &line);
    if (!ok){
      ///chuyển về control
    }
    return;
  }
  float error = line.getLineError();
  int *speeds = lg.computeFE(error, motor.getBaseSpeed());
  motor.move(speeds[0], speeds[1]);
  oled.printMotor(speeds);
  oled.printLineError(dist);

  socket.boardcastLine(String(dist));
}

void setup()
{

  Serial.begin(115200);
  while (!Serial)
    delay(10);
  rom.init();
  Wire.begin(21, 22);
  oled.init();
  oled.showBar(0);

  battery.init();
  oled.showBar(12);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, 0);
  oled.showBar(24);

  imu.init(&rom);
  imu.update();
  oled.showBar(36);

  line.init(&rom);
  oled.setLine(line.getLowLine(), line.getHighLine());
  oled.showBar(48);

  WiFi.softAP(wifiSsid, wifiPassword);
  delay(1000);
  oled.showBar(60);
  lg.init();

  socket.initSocket(&line, &motor);
  oled.showBar(80);

  web.begin();
  delay(1000);
  oled.showBar(100);
  oled.hello();
}

void loop()
{
  web.handle();
  socket.soketLoop();
  SocketState state = socket.getState();
  oled.clear();
  switch (state)
  {
  case LINE_FOLOW:
  {
    if (pevState != state)
    {
      oled.debug("Start \n FOLOW ");
      lg.startLogic();
    }
    lineForward();
    break;
  }
  case CONTROL:
  {
    int *move = socket.getMove();
    motor.move(move[0], move[1]);
    oled.printMotor(move);
    break;
  }
  case START_CALIBRATE:
  {
    if (pevState != state)
    {
      oled.debug("Start \n CALIBRATE");
      line.calibrateLine(true);
    }
    else
    {
      line.calibrateLine(false);
    }
    int *move = socket.getMove();
    motor.move(move[0], move[1]);
    oled.printMotor(move);
    break;
  }
  case END_CALIBRATE:
  {
    motor.move(0, 0);
    line.saveCalibration();
    socket.emitLine();
    oled.setLine(line.getLowLine(), line.getHighLine());
    break;
  }
  default:
    break;
  }
  pevState = state;

  oled.printState(state);
  oled.printBattery(battery.getBatteryVoltage());
  oled.printLine(line.getRawLine());
  oled.printWifiInfo(wifiSsid, wifiPassword, WiFi.softAPIP().toString());
  oled.displayUpdate();
}
