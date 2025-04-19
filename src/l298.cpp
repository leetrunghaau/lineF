
#include "l298.h"


L298::L298(Imu *imuSensor)
{
    _enableA = enableAPin;
    _enableB = enableBPin;
    _inputA1 = inputA1Pin;
    _inputA2 = inputA2Pin;
    _inputB1 = inputB1Pin;
    _inputB2 = inputB2Pin;
    _imu = imuSensor;

    pinMode(_enableA, OUTPUT);
    pinMode(_enableB, OUTPUT);
    pinMode(_inputA1, OUTPUT);
    pinMode(_inputA2, OUTPUT);
    pinMode(_inputB1, OUTPUT);
    pinMode(_inputB2, OUTPUT);
}

void L298::stop()
{
    digitalWrite(_enableA, LOW);
    digitalWrite(_enableB, LOW);
};

void L298::forward(int speed)
{
    digitalWrite(_inputA1, HIGH);
    digitalWrite(_inputA2, LOW);
    digitalWrite(_inputB1, HIGH);
    digitalWrite(_inputB2, LOW);
    analogWrite(_enableA, speed);
    analogWrite(_enableB, speed);
};

void L298::backward(int speed)
{
    digitalWrite(_inputA1, LOW);
    digitalWrite(_inputA2, HIGH);
    digitalWrite(_inputB1, LOW);
    digitalWrite(_inputB2, HIGH);
    analogWrite(_enableA, speed);
    analogWrite(_enableB, speed);
};

void L298::left(int speed)
{
    digitalWrite(_inputA1, HIGH);
    digitalWrite(_inputA2, LOW);
    digitalWrite(_inputB1, LOW);
    digitalWrite(_inputB2, HIGH);
    analogWrite(_enableA, speed);
    analogWrite(_enableB, 0);
};

void L298::right(int speed)
{
    digitalWrite(_inputA1, LOW);
    digitalWrite(_inputA2, HIGH);
    digitalWrite(_inputB1, HIGH);
    digitalWrite(_inputB2, LOW);
    analogWrite(_enableA, 0);
    analogWrite(_enableB, speed);
};
void L298::move(int leftSpeed, int rightSpeed)
{
    if (leftSpeed > 0)
    {
        digitalWrite(_inputA1, HIGH);
        digitalWrite(_inputA2, LOW);
    }
    else if (leftSpeed < 0)
    {
        digitalWrite(_inputA1, LOW);
        digitalWrite(_inputA2, HIGH);
    }
    else
    {
        digitalWrite(_inputA1, LOW);
        digitalWrite(_inputA2, LOW);
    }

    if (rightSpeed > 0)
    {
        digitalWrite(_inputB1, HIGH);
        digitalWrite(_inputB2, LOW);
    }
    else if (rightSpeed < 0)
    {
        digitalWrite(_inputB1, LOW);
        digitalWrite(_inputB2, HIGH);
    }
    else
    {
        digitalWrite(_inputB1, LOW);
        digitalWrite(_inputB2, LOW);
    }

    analogWrite(_enableA, abs(leftSpeed));
    analogWrite(_enableB, abs(rightSpeed));
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
        _imu->update(); // Cập nhật imu liên tục
        float currentYaw = _imu->getYaw();

        // Xử lý vòng xoay qua 0°
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

L298::~L298()
{
}
