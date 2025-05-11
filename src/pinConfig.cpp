#include <Arduino.h>
#include "pinConfig.h"

String SocketStateName[10] = {
    "DISCONNECTED",
    "LINE_FOLOW",
    "CONTROL",
    "CALIBRATTING",
    "CALIBRATTING",
    "END_CALIBRATE"
};