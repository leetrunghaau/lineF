#pragma once

#include <WiFi.h>
#include <WebSocketsServer.h>
#include "pinConfig.h"



class Socket
{
private:
    static bool _isConnected;
    static int _speed;
    static int _codes;
    static int _mode;
    static WebSocketsServer webSocket;
    static void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

public:
    Socket();
    void initSocket();
    void soketLoop();
    ~Socket();
};
