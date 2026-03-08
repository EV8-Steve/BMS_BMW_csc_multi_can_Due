#pragma once

#include <Arduino.h>

#define CAN_TIMEOUT 2000

class CANWatchdog
{

public:

    void init();

    void frameReceived(uint8_t bus);

    void update();

    bool fault();

    void clearFault();

private:

    uint32_t lastFrame[2];
    bool canFault;

};