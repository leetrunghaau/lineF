#include "socket.h"
#include <ArduinoJson.h>
#include "helper.h"

int Socket::_runControl = 0;
int Socket::_turnControl = 0;
bool Socket::_isConnected = false;
Line *Socket::_line = nullptr; 
WebSocketsServer Socket::webSocket = WebSocketsServer(81);

Socket::Socket() {}

void Socket::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        String message = String((char *)payload); 
        JsonDocument storage;
        deserializeJson(storage, message);
        _runControl = storage["run"].as<int>();
        _turnControl = storage["turn"].as<int>();
        webSocket.sendTXT(num, converseLine(_line->getRawLine()).c_str()); 
    }
    else if (type == WStype_DISCONNECTED)
    {
        _isConnected = webSocket.connectedClients() > 0; 
    }

    else if (type == WStype_CONNECTED)
    {
        webSocket.sendTXT(num, converseConnect(_line->getLowLine(), _line->getHighLine(), _line->getRawLine()).c_str()); 
        _isConnected = true;
    }
}


void Socket::initSocket(Line *line )
{
    _line = line; 
    webSocket.begin(); 
    webSocket.onEvent(handleWebSocketMessage); 
}

void Socket::soketLoop()
{
    webSocket.loop();
}
bool Socket::isConnected()
{
    return _isConnected;
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
Socket::~Socket()
{
    webSocket.close();
    _isConnected = false;
}
