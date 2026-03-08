#include "CSCAddressManager.h"

#include <due_can.h>

extern CANRaw Can0;
extern CANRaw Can1;



/*
ID counters for each CAN bus
bus 0 -> nextID[0]
bus 1 -> nextID[1]
*/
uint8_t nextID[2] = {0,0};



CSCAddressManager::CSCAddressManager()
{
}


/*
================================================
Send frame helper
================================================
*/

void CSCAddressManager::sendFrame(uint8_t bus, CAN_FRAME &frame)
{

    if(bus == 0)
        Can0.sendFrame(frame);
    else
        Can1.sendFrame(frame);

}



/*
================================================
Renumber bus
================================================
*/

void CSCAddressManager::renumberBus(uint8_t bus)
{

    CAN_FRAME msg;

    Serial.print("Starting CSC renumber on bus ");
    Serial.println(bus);


    /*
    --------------------------------
    Reset all module IDs
    --------------------------------
    */

    Serial.println("Resetting CSC IDs");

    for (int ID = 0; ID < 15; ID++)
    {

        msg.id  = 0x0A0;
        msg.length = 8;

        msg.data.bytes[0] = 0xA1;
        msg.data.bytes[1] = ID;

        for(int i=2;i<8;i++)
            msg.data.bytes[i] = 0xFF;

        sendFrame(bus,msg);

        delay(2);

    }



    /*
    --------------------------------
    Wait for modules to reset
    --------------------------------
    */

    delay(100);



    /*
    --------------------------------
    Request unassigned modules
    --------------------------------
    */

    nextID[bus] = 0;

    msg.id = 0x0A0;
    msg.length = 8;

    msg.data.bytes[0] = 0x37;

    for(int i=1;i<8;i++)
        msg.data.bytes[i] = 0xFF;

    sendFrame(bus,msg);

}



/*
================================================
Handle unassigned module
================================================
*/

void CSCAddressManager::handleUnassigned(uint8_t bus, CAN_FRAME &frame)
{
    if(discoveryLocked)
        return;

    CAN_FRAME msg;

    uint8_t DMC[8];

    for(int i=0;i<8;i++)
        DMC[i] = frame.data.bytes[i];



    /*
    --------------------------------
    Assign ID to module
    --------------------------------
    */

    msg.id = 0x0A0;
    msg.length = 8;

    msg.data.bytes[0] = 0x12;
    msg.data.bytes[1] = 0xAB;

    msg.data.bytes[2] = DMC[0];
    msg.data.bytes[3] = DMC[1];
    msg.data.bytes[4] = DMC[2];
    msg.data.bytes[5] = DMC[3];

    msg.data.bytes[6] = 0xFF;
    msg.data.bytes[7] = 0xFF;

    sendFrame(bus,msg);

    delay(30);



    msg.data.bytes[1] = 0xBA;
    msg.data.bytes[2] = DMC[4];
    msg.data.bytes[3] = DMC[5];
    msg.data.bytes[4] = DMC[6];
    msg.data.bytes[5] = DMC[7];

    sendFrame(bus,msg);

    delay(10);



    /*
    --------------------------------
    Set module ID
    --------------------------------
    */

    if(nextID[bus] >= 6)
    {
        Serial.println("Maximum CSC IDs reached on this bus");
        return;
    }
    msg.data.bytes[0] = 0x5B;
    msg.data.bytes[1] = nextID[bus];

    sendFrame(bus,msg);

    delay(10);



    msg.data.bytes[0] = 0x37;
    msg.data.bytes[1] = nextID[bus];

    sendFrame(bus,msg);



    Serial.print("CSC assigned ID ");
    Serial.print(nextID[bus]);
    Serial.print(" on bus ");
    Serial.println(bus);



    nextID[bus]++;

    /*
    Give module time to store ID
    */

    delay(20);



    /*
    Request next unassigned module
    */

    msg.data.bytes[0] = 0x37;
    msg.data.bytes[1] = 0xFF;

    sendFrame(bus,msg);

}



/*
================================================
Lock discovery
================================================
*/

void CSCAddressManager::lockDiscovery()
{

    Serial.println("CSC discovery locked");

    discoveryLocked = true;

}



/*
================================================
Unlock discovery
================================================
*/

void CSCAddressManager::unlockDiscovery()
{

    Serial.println("CSC discovery unlocked");

    discoveryLocked = false;

}



/*
================================================
Discovery state
================================================
*/

bool CSCAddressManager::isLocked()
{
    return discoveryLocked;
}
