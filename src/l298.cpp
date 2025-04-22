#include "l298.h"

// Kênh PWM cho ESP32 (ESP32 có 16 kênh PWM: 0–15)
#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1
#define PWM_FREQ 1000         // Tần số PWM 1kHz (tùy chọn)
#define PWM_RESOLUTION 8      // Độ phân giải 8 bit → giá trị 0–255

L298::L298()
{
    _enableA = enableAPin;
    _enableB = enableBPin;
    _inputA1 = inputA1Pin;
    _inputA2 = inputA2Pin;
    _inputB1 = inputB1Pin;
    _inputB2 = inputB2Pin;
    _imu = nullptr;

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

void L298::stop()
{
    ledcWrite(PWM_CHANNEL_A, 0);
    ledcWrite(PWM_CHANNEL_B, 0);
}

void L298::forward(int speed)
{
    digitalWrite(_inputA1, HIGH);
    digitalWrite(_inputA2, LOW);
    digitalWrite(_inputB1, HIGH);
    digitalWrite(_inputB2, LOW);
    ledcWrite(PWM_CHANNEL_A, speed);
    ledcWrite(PWM_CHANNEL_B, speed);
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
    digitalWrite(_inputA1, HIGH);
    digitalWrite(_inputA2, LOW);
    digitalWrite(_inputB1, LOW);
    digitalWrite(_inputB2, HIGH);
    ledcWrite(PWM_CHANNEL_A, speed);
    ledcWrite(PWM_CHANNEL_B, 0);
}

void L298::right(int speed)
{
    digitalWrite(_inputA1, LOW);
    digitalWrite(_inputA2, HIGH);
    digitalWrite(_inputB1, HIGH);
    digitalWrite(_inputB2, LOW);
    ledcWrite(PWM_CHANNEL_A, 0);
    ledcWrite(PWM_CHANNEL_B, speed);
}

void L298::move(int leftSpeed, int rightSpeed)
{
    // Set chiều quay
    digitalWrite(_inputA1, leftSpeed > 0);
    digitalWrite(_inputA2, leftSpeed < 0);
    digitalWrite(_inputB1, rightSpeed > 0);
    digitalWrite(_inputB2, rightSpeed < 0);

    ledcWrite(PWM_CHANNEL_A, abs(leftSpeed));
    ledcWrite(PWM_CHANNEL_B, abs(rightSpeed));
}
void L298::turnLeftAngle(int angle, int speed)
{
    float startYaw = _imu->getYaw();
    float targetYaw = startYaw - angle;
    if (targetYaw < 0)
        targetYaw += 360;
    this->left(speed);
    while (true)
    {
        _imu->update();
        float currentYaw = _imu->getYaw();
        float diff = startYaw - currentYaw;
        if (diff < 0)
            diff += 360;
        if (diff >= angle)
            break;
        delay(10);
    }
    this->stop();
}
void L298::turnRightAngle(int angle, int speed)
{
    float startYaw = _imu->getYaw();
    float targetYaw = startYaw + angle;
    if (targetYaw >= 360)
        targetYaw -= 360;
    this->right(speed);
    while (true)
    {
        _imu->update();
        float currentYaw = _imu->getYaw();
        float diff = currentYaw - startYaw;
        if (diff < 0)
            diff += 360;
        if (diff >= angle)
            break;
        delay(10);
    }
    this->stop();
}
void L298::setImu(Imu *imuSensor)
{
    _imu = imuSensor;
}
L298::~L298() {
    
  }