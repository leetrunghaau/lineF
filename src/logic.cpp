#include "logic.h"
#include <Arduino.h>

Logic::Logic()
{
}
void Logic::init()
{
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;
  _integral = 0;
  _lastError = 0;
  _lineIsGlitch = 0;
}

int *Logic::compute(float lineError, int baseSpeed)
{
  int speed = baseSpeed;
  static int motorSpeeds[2];
  if (lineError >= 99.0)
  {
    // cảnh báo ở chổ này
    _lineIsGlitch++;
    if (_lineIsGlitch > 5)
    {
      motorSpeeds[0] = 0;
      motorSpeeds[1] = 0;
      return motorSpeeds;
    }
    else
    {

      motorSpeeds[0] = speed;
      motorSpeeds[1] = speed;
      return motorSpeeds;
    }
  }
  else
  {
    _lineIsGlitch = 0;
  }

  // Logic Calculation
  _integral += lineError;
  float derivative = lineError - _lastError;
  _lastError = lineError;
  float correction = _Kp * lineError + _Ki * _integral + _Kd * derivative;
  int motorLeft = speed - (correction > 0 ? correction * 3.5 : correction * 0.7);
  int motorRight = speed + (correction < 0 ? correction * 3.5 : correction * 0.7);
  motorLeft = constrain(motorLeft, -maxSpeed, maxSpeed);
  motorRight = constrain(motorRight, -maxSpeed, maxSpeed);

  motorSpeeds[0] = motorLeft;
  motorSpeeds[1] = motorRight;

  return motorSpeeds;
}
int *Logic::computeFE(float lineError, int baseSpeed)
{
  int speed = baseSpeed;
  static int motorSpeeds[2];
  if (lineError >= 99.0)
  {
    // cảnh báo ở chổ này
    _lineIsGlitch++;
    if (_lineIsGlitch > 5)
    {
      motorSpeeds[0] = 0;
      motorSpeeds[1] = 0;
      return motorSpeeds;
    }
    else
    {
      motorSpeeds[0] = speed;
      motorSpeeds[1] = speed;
      return motorSpeeds;
    }
  }
  else
  {
    _lineIsGlitch = 0;
  }
  if (abs(lineError) > maxError)
  {
    // _lastError = _lastError >= 1 ? 2 : _lastError;
    // _lastError = _lastError <= -1 ? -2 : _lastError;
    lineError = _lastError;
  }

  _lastError = lineError;
  int motorLeft = speed;
  int motorRight = speed;
  if (lineError == 0.5)
  {
    motorLeft = speed / 1.5;
    motorRight = speed;
  }
  else if (lineError == -0.5)
  {
    motorLeft = speed;
    motorRight = speed / 1.5;
  }
  else if (lineError == 1)
  {
    motorLeft = 0;
    motorRight = speed;
  }
  else if (lineError == -1)
  {
    motorLeft = speed;
    motorRight = 0;
  }
  else if (lineError == 1.5)
  {
    motorLeft = -speed / 2;
    motorRight = speed;
  }
  else if (lineError == -1.5)
  {
    motorLeft = speed;
    motorRight = -speed / 2;
  }
  else if (lineError == 2)
  {
    motorLeft = -speed * 1.3;
    motorRight = speed;
  }
  else if (lineError == -2)
  {
    motorLeft = speed;
    motorRight = -speed * 1.3;
  }

  motorSpeeds[0] = motorLeft;
  motorSpeeds[1] = motorRight;

  return motorSpeeds;
}
void Logic::startLogic()
{
  _integral = 0;
  _lastError = 0;
}
float Logic::getLeftDistance(L298 *motor, UltraSonicDistanceSensor *sonic)
{
  motor->turnLeft45();
  delay(200);
  float d = sonic->measureDistanceCm();
  motor->turnRight45();
  delay(200);
  return d;
}

float Logic::getRightDistance(L298 *motor, UltraSonicDistanceSensor *sonic)
{
  motor->turnRight45();
  delay(200);
  float d = sonic->measureDistanceCm();
  motor->turnLeft45();
  delay(200);
  return d;
}
void Logic::splitLine(bool direction, L298 *motor, Oled *oled)
{
  if (direction)
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("                   \n") +
            String("                   \n") +
            String("        ##         \n") +
            String("        ##         \n") +
            String("                   \n") +
            String("        #######>   \n"),
        1);
    motor->turnRight90();
  }
  else
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("                   \n") +
            String("                   \n") +
            String("        ##         \n") +
            String("        ##         \n") +
            String("                   \n") +
            String("  <#######         \n"),
        1);
    motor->turnLeft90();
  }
  delay(1000);
  motor->forwardAvoid(true);
  delay(1000);
}
void Logic::parallelLine(bool direction, L298 *motor, Oled *oled)
{
  oled->debug("Obstacle\n\ndi song song", 1);
  if (direction)
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("               ^   \n") +
            String("               #   \n") +
            String("        ##     #   \n") +
            String("        ##     #   \n") +
            String("               #   \n") +
            String("        ########   \n"),
        1);
    motor->turnLeft90();
  }
  else
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("  ^                \n") +
            String("  #                \n") +
            String("  #     ##         \n") +
            String("  #     ##         \n") +
            String("  #                \n") +
            String("  ########         \n"),
        1);
    motor->turnRight90();
  }
  delay(1000);
  motor->forwardAvoid();
  delay(1000);
}
bool Logic::mergeLine(bool direction, L298 *motor, Oled *oled, Line *line)
{
  oled->debug("Obstacle\n\ntim line", 1);
  if (direction)
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("        <#######   \n") +
            String("               #   \n") +
            String("        ##     #   \n") +
            String("        ##     #   \n") +
            String("               #   \n") +
            String("        ########   \n"),
        1);
    motor->turnLeft90();
  }
  else
  {
    oled->debug(
        String("Obstacle\n") +
            String("                   \n") +
            String("  #######>         \n") +
            String("  #                \n") +
            String("  #     ##         \n") +
            String("  #     ##         \n") +
            String("  #                \n") +
            String("  ########         \n"),
        1);
    motor->turnRight90();
  }
  int attempts = 0;
  motor->stop();
  delay(1000);
  const int maxAttempts = 800;
  while (attempts < maxAttempts)
  {
    motor->forward(motor->getBaseSpeed());
    float error = line->getLineError();
    LineState linestate = line->getLineState();

    if (linestate != LINE_LOST)
    {
      motor->stop();
      delay(1000);
      break;
    }
    attempts++;
  }

  oled->debug("Obstacle\n\nnhap line", 1);
  if (attempts < maxAttempts)
  {
    if (direction)
    {
      oled->debug(
          String("Obstacle\n") +
              String("        ^          \n") +
              String("        ########   \n") +
              String("               #   \n") +
              String("        ##     #   \n") +
              String("        ##     #   \n") +
              String("               #   \n") +
              String("        ########   \n"),
          1);
      motor->turnRight90(true);
    }
    else
    {
      oled->debug(
          String("Obstacle\n") +
              String("         ^         \n") +
              String("  ########         \n") +
              String("  #                \n") +
              String("  #     ##         \n") +
              String("  #     ##         \n") +
              String("  #                \n") +
              String("  ########         \n"),
          1);
      motor->turnLeft90(true);
    }
  }
  else
  {
    motor->stop();
    oled->debug(
        String("Obstacle\n") +
        String("                   \n") +
        String("      #    #       \n") +
        String("       #  #        \n") +
        String("        ##         \n") +
        String("        ##         \n") +
        String("       #  #        \n") +
        String("      #    #       \n"));
    return false;
  }
  motor->stop();
  delay(100);
  return true;
}
bool Logic::avoidObstacle(L298 *motor, UltraSonicDistanceSensor *sonic, Oled *oled, Line *line)
{
  //===========================================
  oled->debug("Obstacle", 1);
  motor->stop();
  delay(800);
  //===========================================
  float leftDist = getLeftDistance(motor, sonic);
  oled->debug(
      String("Obstacle\n") +
          String("                   \n") +
          String("                   \n") +
          String("                   \n") +
          String("        ##         \n") +
          String(leftDist) + String("\n") +
          String("                   \n") +
          String("                   \n"),
      1);
  float rightDist = getRightDistance(motor, sonic);
  oled->debug(
      String("Obstacle\n") +
      String("                   \n") +
      String("                   \n") +
      String("                   \n") +
      String("        ##         \n") +
      String(leftDist) + String("           ") + String(rightDist) + String("\n") +
      String("                   \n") +
      String("                   \n"));
  //===========================================
  const float minSafeDist = 20.0;
  bool canGoLeft = leftDist > minSafeDist;
  bool canGoRight = rightDist > minSafeDist;
  //===========================================
  if (!canGoLeft && !canGoRight)
  {
    motor->stop();
    oled->debug(
        String("Obstacle\n") +
        String("                   \n") +
        String("      #    #       \n") +
        String("       #  #        \n") +
        String("        ##         \n") +
        String("        ##         \n") +
        String("       #  #        \n") +
        String("      #    #       \n"));
    return false;
  }

  // chọn hướng
  bool turnRight;
  if (canGoRight && !canGoLeft)
    turnRight = true;
  else if (!canGoRight && canGoLeft)
    turnRight = false;
  else
    turnRight = rightDist > leftDist;

  this->splitLine(turnRight, motor, oled);
  this->parallelLine(turnRight, motor, oled);
  return this->mergeLine(turnRight, motor, oled, line);
}
Logic::~Logic()
{
}