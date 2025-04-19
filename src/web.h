#pragma once

#include <WebServer.h>
#include <Arduino.h>

class Web
{
private:
    WebServer server;

public:
    Web();
    ~Web();
    void begin();
    void handle();
};
