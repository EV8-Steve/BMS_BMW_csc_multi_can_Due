#pragma once

#include <Arduino.h>
#include <due_can.h>

class CSCAddressManager
{

public:

    CSCAddressManager();

    /*
    ================================================
    CSC Discovery / Address Assignment
    ================================================
    */

    void renumberBus(uint8_t bus);

    void handleUnassigned(uint8_t bus, CAN_FRAME &frame);



    /*
    ================================================
    Discovery Control
    ================================================
    */

    void lockDiscovery();

    void unlockDiscovery();

    bool isLocked();



private:

    /*
    ================================================
    CAN Send Helper
    ================================================
    */

    void sendFrame(uint8_t bus, CAN_FRAME &frame);



    /*
    ================================================
    Discovery State
    ================================================
    */

    bool discoveryLocked;

};