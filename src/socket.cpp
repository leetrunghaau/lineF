#include "socket.h"

bool Socket::_isConnected = false;
WebSocketsServer Socket::webSocket = WebSocketsServer(81);

Socket::Socket() {}

void Socket::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        String message = String((char *)payload); 
        int separatorIndex = message.indexOf('|');

        if (separatorIndex > 0)
        {
            String command = message.substring(0, separatorIndex);
            String value = message.substring(separatorIndex + 1);
            
            if (command == "speed") {
                _speed = value.toInt();
            } else if (command == "mode") {
                _mode = value.toInt();
            } else if (command == "control") {
                _codes = value.toInt();
            }
        }
        else
        {
            Serial.println("⚠️ Định dạng sai, không tìm thấy ký tự |");
        }

        webSocket.sendTXT(num, "ESP32 đã nhận: " + message);
    }

    else if (type == WStype_DISCONNECTED)
    {
        _isConnected = false;
    }

    else if (type == WStype_CONNECTED)
    {
        _isConnected = true;
    }
}


void Socket::initSocket()
{
    webSocket.begin();
    webSocket.onEvent(handleWebSocketMessage); // static function được chấp nhận
}

void Socket::soketLoop()
{
    webSocket.loop();
}

Socket::~Socket()
{
    webSocket.close();
    _isConnected = false;
}
