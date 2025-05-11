#pragma once

#include <WiFi.h>
#include <WebSocketsServer.h>
#include "pinConfig.h"
#include "line.h"
#include "l298.h"


class Socket
{
private:
    static SocketState _state;
    static SocketState _prevState;
    static int _runControl;
    static int _turnControl;
    static WebSocketsServer webSocket;
    static Line *_line;
    static L298 *_motor ;
    static void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

public:
    Socket();
    void initSocket(Line *line = nullptr, L298 *_motor = nullptr);
    void soketLoop();
    void emitLine();
    void boardcastLine(String fb ="");
    int getRun();
    int getTurn();
    SocketState getState();
    int *getMove();
    bool isConnected();
    void setLine(Line *line);
    ~Socket();
};
