#include "l298.h"

// Kênh PWM cho ESP32 (ESP32 có 16 kênh PWM: 0–15)
#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1
#define PWM_FREQ 1000    // Tần số PWM 1kHz (tùy chọn)
#define PWM_RESOLUTION 8 // Độ phân giải 8 bit → giá trị 0–255

L298::L298()
{
    _battery.init();
    _t45 = time45;
    _t90 = time90;
    _timeForward = timeForward;
    _baseVolt = baseVolt;
    _enableA = enableAPin;
    _enableB = enableBPin;
    _inputA1 = inputA1Pin;
    _inputA2 = inputA2Pin;
    _inputB1 = inputB1Pin;
    _inputB2 = inputB2Pin;

    // Thiết lập các chân output
    pinMode(_inputA1, OUTPUT);
    pinMode(_inputA2, OUTPUT);
    pinMode(_inputB1, OUTPUT);
    pinMode(_inputB2, OUTPUT);

    // Thiết lập PWM cho ESP32
    ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(_enableA, PWM_CHANNEL_A);
    ledcAttachPin(_enableB, PWM_CHANNEL_B);
}
void L298::setValue(int t45, int t90, int TF, float baseV)
{
    _t45 = t45;
    _t90 = t90;
    _timeForward = TF;
    _baseVolt = baseV;
}
int L298::getBaseSpeed()
{
    float desiredVolt = _baseVolt;
    int speed = (int)((desiredVolt / _battery.getBatteryVoltage()) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    return speed;
}
int L298::getT90()
{

    return _t90;
}
void L298::forwardAvoid(bool sub)
{
    this->forward(this->getBaseSpeed());
    if (sub)
    {
        delay(_timeForward * 0.6);
    }
    else
    {
        delay(_timeForward);
    }
    this->stop();
}
void L298::stop()
{
    ledcWrite(PWM_CHANNEL_A, 0);
    ledcWrite(PWM_CHANNEL_B, 0);
}

void L298::forward(int speed)
{
    this->move(speed, speed);
}

void L298::backward(int speed)
{
    digitalWrite(_inputA1, LOW);
    digitalWrite(_inputA2, HIGH);
    digitalWrite(_inputB1, LOW);
    digitalWrite(_inputB2, HIGH);
    ledcWrite(PWM_CHANNEL_A, speed);
    ledcWrite(PWM_CHANNEL_B, speed);
}
void L298::left(int speed)
{
    this->move(-speed, speed);
}
void L298::right(int speed)
{
    this->move(speed, -speed);
}

void L298::move(int leftSpeed, int rightSpeed)
{
    // Set chiều quay
    digitalWrite(_inputA1, leftSpeed > 0);
    digitalWrite(_inputA2, leftSpeed < 0);
    digitalWrite(_inputB1, rightSpeed > 0);
    digitalWrite(_inputB2, rightSpeed < 0);

    ledcWrite(PWM_CHANNEL_A, abs(leftSpeed) > 30 ? abs(leftSpeed) : 0);
    ledcWrite(PWM_CHANNEL_B, abs(rightSpeed) > 30 ? abs(rightSpeed) : 0);
}

void L298::turnLeft90(bool lock)
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);

    if (lock)
    {
        this->move(0, speed);
        delay(_t90 * 2);
    }
    else
    {
        this->move(-speed, speed);
        delay(_t90);
    }
    this->stop();
}
void L298::turnLeft45()
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    this->move(-speed, speed);
    delay(_t45);
    this->stop();
}
void L298::turnRight90(bool lock)
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    if (lock)
    {
        this->move(speed, 0);
        delay(_t90 * 2);
    }
    else
    {
        this->move(speed, -speed);
        delay(_t90);
    }

    this->stop();
}
void L298::turnRight45()
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    this->move(speed, -speed);
    delay(_t45);
    this->stop();
}
void L298::test(Oled *oled)
{
    for (int i = 500; i < 1200; i += 100)
    {
        oled->debug("left " + String(i));
        this->testLeft(i);
        delay(3000);
        oled->debug("right " + String(i));
        this->testRigh(i);
        delay(3000);
    }
}
void L298::testLeft(int time)
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    this->move(-speed, speed);
    delay(time);
    this->stop();
}
void L298::testRigh(int time)
{
    float curVolt = _battery.getBatteryVoltage();
    float desiredVolt = 3.4;
    int speed = (int)((desiredVolt / curVolt) * 386.3636);
    speed = constrain(speed, 0, maxSpeed);
    this->move(speed, -speed);
    delay(time);
    this->stop();
}
L298::~L298()
{
}