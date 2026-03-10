#pragma once

#include <Arduino.h>
#include "due_can.h"

class OutlanderChargerStatus
{

public:

    void processCAN(CAN_FRAME &frame);

    float chargerVoltage = 0;
    float dcCurrent = 0;
    float acCurrent = 0;

    float temp1 = 0;
    float temp2 = 0;
    float temp3 = 0;

    float auxVoltage = 0;

    uint8_t status = 0;

};