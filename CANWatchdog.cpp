#include "CANWatchdog.h"

void CANWatchdog::init()
{
    lastFrame[0] = millis();
    lastFrame[1] = millis();
    canFault = false;
}

void CANWatchdog::frameReceived(uint8_t bus)
{
    lastFrame[bus] = millis();
}

void CANWatchdog::update()
{
    uint32_t now = millis();

    for(int i=0;i<2;i++)
    {
        if(now - lastFrame[i] > CAN_TIMEOUT)
            canFault = true;
    }
}

bool CANWatchdog::fault()
{
    return canFault;
}

void CANWatchdog::clearFault()
{
    canFault = false;
}
