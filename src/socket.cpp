#include "socket.h"
#include <ArduinoJson.h>
#include "helper.h"

SocketState Socket::_state = DISCONNECTED;
SocketState Socket::_prevState = DISCONNECTED;
int Socket::_runControl = 0;
int Socket::_turnControl = 0;
Line *Socket::_line = nullptr;
L298 *Socket::_motor = nullptr;
WebSocketsServer Socket::webSocket = WebSocketsServer(81);

Socket::Socket() {}

void Socket::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        String message = String((char *)payload);
        Serial.println(message);
        JsonDocument storage;
        deserializeJson(storage, message);
        String state = storage["type"].as<String>();

        if (state == "control")
        {
            _state = CONTROL;
        }
        else if (state == "follow")
        {
            _state = LINE_FOLOW;
        }
        else if (state == "calibrate")
        {
            _state = START_CALIBRATE;
        }
        else if (state == "saveCalibrate")
        {
            _state = END_CALIBRATE;
        }
        else if (state == "setBase")
        {
            _motor->setValue(storage["T45"].as<int>(), storage["T90"].as<int>(), storage["TF"].as<int>(), storage["speed"].as<float>());
            digitalWrite(buzzerPin, 1);
            delay(1000);
            digitalWrite(buzzerPin, 0);
            return;
        }
        else if (state == "TF1")
        {
            _motor->turnRight45();
            return;
        }
        else if (state == "TF2")
        {
            _motor->turnRight90();
            return;
        }
        else if (state == "TF3")
        {
            _motor->forwardAvoid();
          
            return;
        }

        _runControl = storage["run"].as<int>();
        _turnControl = storage["turn"].as<int>();
        webSocket.sendTXT(num, converseLine(_line->getRawLine()).c_str());
    }
    else if (type == WStype_DISCONNECTED)
    {
        _state = (webSocket.connectedClients() > 0) ? _state : DISCONNECTED;
        _runControl = 0;
        _turnControl = 0;
    }
    else if (type == WStype_CONNECTED)
    {
        webSocket.sendTXT(num, converseConnected(_line->getLowLine(), _line->getHighLine(), _line->getRawLine()).c_str());
        _state = CONTROL;
        _runControl = 0;
        _turnControl = 0;
    }
}

void Socket::initSocket(Line *line, L298 *motor)
{
    _line = line;
    _motor = motor;
    webSocket.begin();
    webSocket.onEvent(handleWebSocketMessage);
}
void Socket::emitLine()
{
    _state = CONTROL;
    webSocket.broadcastTXT(converseCalibrated(_line->getLowLine(), _line->getHighLine(), _line->getRawLine()).c_str());
}
void Socket::boardcastLine(String fb)
{
    webSocket.broadcastTXT(converseLine(_line->getRawLine(), fb).c_str());
}
void Socket::soketLoop()
{

    webSocket.loop();
}
bool Socket::isConnected()
{
    return _state != DISCONNECTED;
}
void Socket::setLine(Line *line)
{
    _line = line;
}
int Socket::getRun()
{
    return _runControl;
}
int Socket::getTurn()
{
    return _turnControl;
}
SocketState Socket::getState()
{
    return _state;
}

int *Socket::getMove()
{
    static int result[2];
    int speed = constrain(_runControl, -maxSpeed, maxSpeed);
    int turnSpeed = constrain(_turnControl, -maxSpeed, maxSpeed);

    result[0] = speed + turnSpeed;
    result[1] = speed - turnSpeed;

    int maxVal = max(abs(result[0]), abs(result[1]));
    if (maxVal > maxSpeed)
    {
        float scale = float(maxSpeed) / maxVal;
        result[0] *= scale;
        result[1] *= scale;
    }

    return result;
}
Socket::~Socket()
{
    webSocket.close();
}
