#pragma once

#include <WiFi.h>
#include <WebSocketsServer.h>
#include "pinConfig.h"
#include "line.h"

class Socket
{
private:
    static bool _isConnected;
    static int _runControl;
    static int _turnControl;
    static WebSocketsServer webSocket;
    static Line *_line ; // Pointer to Line object
    static void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

public:
    Socket();
    void initSocket(Line *line = nullptr);
    void soketLoop();
    int getRun();
    int getTurn();
    bool isConnected();
    ~Socket();
};
